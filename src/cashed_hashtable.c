#include<cashed_hashtable_def.h>

#include<cashed_jenkinshash.h>
#include<cashed_data.h>

#include<rwlock.h>

#include<stdlib.h>

cashtable* get_cashtable(unsigned int bucket_count)
{
	cashtable* cashtable_p = malloc(sizeof(cashtable));
	init_cashtable(cashtable_p, bucket_count);
	return cashtable_p;
}

void init_cashtable(cashtable* cashtable_p, unsigned int bucket_count)
{
	// number of buckets in the cashtable
	cashtable_p->bucket_count = bucket_count;

	// total number of elements in the cashtable, and lock to protect it
	pthread_mutex_init(&(cashtable_p->data_count_lock), NULL);
	cashtable_p->data_count = 0;

	// create buckets
	cashtable_p->buckets = calloc(cashtable_p->bucket_count, sizeof(c_bucket));
	for(unsigned int i = 0; i < cashtable_p->bucket_count; i++)
		init_bucket(cashtable_p->buckets + i);

	// create data memory manager for the cashtable
	init_data_manager(&(cashtable_p->data_memory_manager), 3 * sizeof(c_data), 128, 33);
}

int get_value_cashtable(cashtable* cashtable_p, const dstring* key, dstring* return_value)
{
	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	read_lock(&(bucket->data_list_lock));

	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key, NULL);

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

int set_key_value_cashtable(cashtable* cashtable_p, const dstring* key, const dstring* value)
{
	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;
	
	write_lock(&(bucket->data_list_lock));

	c_data* prev = NULL;
	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key, &prev);
		
	unsigned int size_of_new_data = get_required_size_of_data(key, value);
	int new_allocation_and_insertion_required = 0;

	if(data_found != NULL)
	{
		if(get_total_size_of_data(data_found) < size_of_new_data)
		{
			remove_bucket_data_next_of_unsafe(bucket, prev);
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
		c_data* new_data = malloc(size_of_new_data);
		init_data(new_data, NULL);
		insert_bucket_head_unsafe(bucket, new_data);
		write_lock(&(new_data->data_value_lock));
		write_unlock(&(bucket->data_list_lock));
		set_data_key_value(new_data, key, value);
		write_unlock(&(new_data->data_value_lock));
	}

	return 1;
}

int del_key_value_cashtable(cashtable* cashtable_p, const dstring* key)
{
	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	write_lock(&(bucket->data_list_lock));

	c_data* prev = NULL;
	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key, &prev);

	if(data_found != NULL)
		remove_bucket_data_next_of_unsafe(bucket, prev);

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

	for(unsigned int i = 0; i < cashtable_p->bucket_count; i++)
		deinit_bucket(cashtable_p->buckets + i);
	free(cashtable_p->buckets);
	cashtable_p->buckets = NULL;

	deinit_data_manager(&(cashtable_p->data_memory_manager));
}

void delete_cashtable(cashtable* cashtable_p)
{
	deinit_cashtable(cashtable_p);
	free(cashtable_p);
}