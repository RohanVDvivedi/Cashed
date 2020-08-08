#include<cashed_hashtable.h>

struct cashbucket
{
	// this lock is responsible for protecting
	// data_list pointer of the bucket and 
	// all the hashtable_next_data pointers in the linked list
	rwlock data_list_lock;

	data* data_list;
};

void init_cashbucket(cashbucket* bucket)
{
	initialize_rwlock(&(bucket->data_list_lock));
	bucket->data_list = NULL;
}

void deinit_cashbucket(cashbucket* bucket)
{
	deinitialize_rwlock(&(bucket->data_list_lock));
	data* data_from_list = bucket->data_list;
	while(data_from_list != NULL)
	{
		data* data_to_delete = data_from_list;
		data_from_list = data_from_list->h_next;
		deinit_data(data_to_delete);
		free(data_to_delete);
	}
	bucket->data_list = NULL;
}

void init_cashtable(cashtable* cashtable_p, unsigned int bucket_count)
{
	cashtable_p->bucket_count = bucket_count;
	pthread_mutex_init(&(cashtable_p->data_count_lock), NULL);
	cashtable_p->data_count = 0;
	cashtable_p->buckets = calloc(cashtable_p->bucket_count, sizeof(cashbucket));
	for(unsigned int i = 0; i < cashtable_p->bucket_count; i++)
		init_cashbucket(cashtable_p->buckets + i);
}

int get_cashtable(cashtable* cashtable_p, const dstring* key, dstring* return_value)
{
	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	cashbucket* bucket = cashtable_p->buckets + index;
	read_lock(&(bucket->data_list_lock));
		data* data_test = bucket->data_list;
		while(data_test != NULL && compare_key(data_test, key) != 0){data_test = data_test->h_next;}
		if(data_test != NULL)
			read_lock(&(data_test->data_value_lock));
	read_unlock(&(bucket->data_list_lock));
		if(data_test != NULL)
		{
				append_data_value(data_test, return_value);
			read_unlock(&(data_test->data_value_lock));
		}
	return data_test != NULL;
}

int set_cashtable(cashtable* cashtable_p, const dstring* key, const dstring* value)
{
	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	cashbucket* bucket = cashtable_p->buckets + index;
	
	write_lock(&(bucket->data_list_lock));
		data* data_test = bucket->data_list;
		data* prev = NULL;

		while(data_test != NULL && compare_key(data_test, key) != 0){prev = data_test;data_test = data_test->h_next;}
		
		unsigned int size_of_new_data = size_of_data(key, value);
		int new_allocation_and_insertion_required = 0;

		if(data_test != NULL)
		{
			if(data_test->total_data_size < size_of_new_data)
			{
				if(prev != NULL)
					prev->h_next = data_test->h_next;
				else
					bucket->data_list = data_test->h_next;
				data_test->h_next = NULL;

				new_allocation_and_insertion_required = 1;
			}
			else
			{
				write_lock(&(data_test->data_value_lock));
					update_value(data_test, value);
				write_unlock(&(data_test->data_value_lock));
			}
		}
		else
			new_allocation_and_insertion_required = 1;

		if(new_allocation_and_insertion_required)
		{
			data* new_data = malloc(size_of_new_data);
			init_data(new_data, size_of_new_data, key, value);
			new_data->h_next = bucket->data_list;
			bucket->data_list = new_data;
		}
	write_unlock(&(bucket->data_list_lock));

	return 1;
}

int del_cashtable(cashtable* cashtable_p, const dstring* key)
{
	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	cashbucket* bucket = cashtable_p->buckets + index;

	write_lock(&(bucket->data_list_lock));
		data* data_test = bucket->data_list;
		data* prev = NULL;
		while(data_test != NULL && compare_key(data_test, key) != 0){prev = data_test;data_test = data_test->h_next;}
		if(data_test != NULL)
		{
			if(prev != NULL)
				prev->h_next = data_test->h_next;
			else
				bucket->data_list = data_test->h_next;
			data_test->h_next = NULL;
		}
	write_unlock(&(bucket->data_list_lock));

	if(data_test != NULL)
	{
		// taking a write lock ensures that noone could be working on this data, now
		write_lock(&(data_test->data_value_lock));
			deinit_data(data_test);
			free(data_test);
	}

	return data_test != NULL;
}

void deinit_cashtable(cashtable* cashtable_p)
{
	pthread_mutex_destroy(&(cashtable_p->data_count_lock));
	if(cashtable_p->buckets != NULL)
	{
		for(unsigned int i = 0; i < cashtable_p->bucket_count; i++)
			deinit_cashbucket(cashtable_p->buckets + i);
		free(cashtable_p->buckets);
		cashtable_p->buckets = NULL;
	}
}
