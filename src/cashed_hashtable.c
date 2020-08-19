#include<cashed_hashtable.h>

#include<jenkinshash.h>
#include<data.h>

#include<rwlock.h>

#include<stdlib.h>

/*
**	STRUCTURE FOR CASHBUCKET
*/

struct cashbucket
{
	// this lock is responsible for protecting
	// data_list pointer of the bucket and 
	// all the hashtable_next_data pointers in the linked list
	rwlock data_list_lock;

	data* data_list;
};

/*
**	FUNCTIONS FOR CASHBUCKET
*/

void init_cashbucket(cashbucket* bucket)
{
	initialize_rwlock(&(bucket->data_list_lock));
	bucket->data_list = NULL;
}

static data* find_cashbucket_data_by_key_unsafe(cashbucket* bucket, const dstring* key, data** prev_return)
{
	data* data_test = bucket->data_list;
	data* prev = NULL;
	while(data_test != NULL && compare_key(data_test, key) != 0)
	{
		prev = data_test;
		data_test = data_test->h_next;
	}
	if(prev_return != NULL)
		*prev_return = prev;
	return data_test;
}

static void insert_cashbucket_head_unsafe(cashbucket* bucket, data* new_data)
{
	new_data->h_next = bucket->data_list;
	bucket->data_list = new_data;
}

// if prev is NULL, then we remove head
// returns the removed node
static data* remove_cashbucket_data_next_of_unsafe(cashbucket* bucket, data* prev)
{
	data* data_ret = NULL;
	if(prev == NULL)
	{
		data_ret = bucket->data_list;
		bucket->data_list = bucket->data_list->h_next;
	}
	else
	{
		data_ret = prev->h_next;
		prev->h_next = prev->h_next->h_next;
	}
	data_ret->h_next = NULL;
	return data_ret;
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

/*
**	FUNCTIONS FOR CASHTABLE
*/

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

	data* data_found = find_cashbucket_data_by_key_unsafe(bucket, key, NULL);

	if(data_found != NULL)
	{
		read_lock(&(data_found->data_value_lock));
		read_unlock(&(bucket->data_list_lock));
		append_data_value(data_found, return_value);
		read_unlock(&(data_found->data_value_lock));
	}
	else
		read_unlock(&(bucket->data_list_lock));

	return data_found != NULL;
}

int set_cashtable(cashtable* cashtable_p, const dstring* key, const dstring* value)
{
	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	cashbucket* bucket = cashtable_p->buckets + index;
	
	write_lock(&(bucket->data_list_lock));

	data* prev = NULL;
	data* data_found = find_cashbucket_data_by_key_unsafe(bucket, key, &prev);
		
	unsigned int size_of_new_data = size_of_data(key, value);
	int new_allocation_and_insertion_required = 0;

	if(data_found != NULL)
	{
		if(data_found->total_data_size < size_of_new_data)
		{
			remove_cashbucket_data_next_of_unsafe(bucket, prev);
			free(data_found);

			new_allocation_and_insertion_required = 1;
		}
		else
		{
			write_lock(&(data_found->data_value_lock));
			write_unlock(&(bucket->data_list_lock));
			update_value(data_found, value);
			write_unlock(&(data_found->data_value_lock));
		}
	}
	else
		new_allocation_and_insertion_required = 1;

	if(new_allocation_and_insertion_required)
	{
		data* new_data = malloc(size_of_new_data);
		init_data(new_data, size_of_new_data, key, value);
		insert_cashbucket_head_unsafe(bucket, new_data);
		write_unlock(&(bucket->data_list_lock));
	}

	return 1;
}

int del_cashtable(cashtable* cashtable_p, const dstring* key)
{
	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	cashbucket* bucket = cashtable_p->buckets + index;

	write_lock(&(bucket->data_list_lock));

	data* prev = NULL;
	data* data_found = find_cashbucket_data_by_key_unsafe(bucket, key, &prev);

	if(data_found != NULL)
		remove_cashbucket_data_next_of_unsafe(bucket, prev);

	write_unlock(&(bucket->data_list_lock));

	if(data_found != NULL)
	{
		// taking a write lock ensures that noone could be working on this data, now
		write_lock(&(data_found->data_value_lock));
		deinit_data(data_found);
		free(data_found);
	}

	return data_found != NULL;
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
