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
	init_data_manager(&(cashtable_p->data_memory_manager), 3 * sizeof(c_data), 30, 35);
}

int get_value_cashtable(cashtable* cashtable_p, const dstring* key, dstring* return_value)
{
	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	read_lock(&(bucket->data_list_lock));

	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key);

	if(data_found != NULL)
	{
		// once the data you must serve has been found,
		// take a lock on it, and read data value into your buffer
		pthread_mutex_lock(&(data_found->data_value_lock));
		read_unlock(&(bucket->data_list_lock));
			append_data_value(data_found, return_value);
			bump_used_data_on_reuse(data_found->data_class, data_found);
		pthread_mutex_unlock(&(data_found->data_value_lock));
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

	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key);
		
	unsigned int size_of_new_data = get_required_size_of_data(key, value);
	int new_allocation_and_insertion_required = 0;
	// this variable lets us know, if we must insert a new data with the corresponding key and value

	// if a data with the same key vaue is found, we check if we could reuse the memory 
	// we wll try and reuse if possible
	if(data_found != NULL)
	{
		pthread_mutex_lock(&(data_found->data_value_lock));
			// check with memory manager, if it could allow us to reuse the data, for new value to the same key
			if(advise_to_reuse_data(&(cashtable_p->data_memory_manager), get_total_size_of_data(data_found), size_of_new_data))
			{
				// if we are updating, we no longer need to keep holding data list lock
				write_unlock(&(bucket->data_list_lock));
					update_value_expiry(data_found, value, -1);
					bump_used_data_on_reuse(data_found->data_class, data_found);
			}
			else
			{
				remove_bucket_data_unsafe(bucket, data_found);
				return_used_data_to_manager(&(cashtable_p->data_memory_manager), data_found);
				new_allocation_and_insertion_required = 1;
			}
		pthread_mutex_unlock(&(data_found->data_value_lock));
	}
	else
		new_allocation_and_insertion_required = 1;

	if(new_allocation_and_insertion_required)
	{
		c_data* new_data = get_cached_data_from_manager(&(cashtable_p->data_memory_manager), size_of_new_data);
		insert_bucket_head_unsafe(bucket, new_data);
		pthread_mutex_lock(&(new_data->data_value_lock));
		write_unlock(&(bucket->data_list_lock));
			set_data_key_value_expiry(new_data, key, value, -1);
			bump_used_data_on_reuse(data_found->data_class, data_found);
		pthread_mutex_unlock(&(new_data->data_value_lock));
	}

	return 1;
}

int del_key_value_cashtable(cashtable* cashtable_p, const dstring* key)
{
	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	write_lock(&(bucket->data_list_lock));

	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key);

	if(data_found != NULL)
	{
		pthread_mutex_lock(&(data_found->data_value_lock));
		remove_bucket_data_unsafe(bucket, data_found);
	}

	write_unlock(&(bucket->data_list_lock));

	if(data_found != NULL)
	{
		return_used_data_to_manager(&(cashtable_p->data_memory_manager), data_found);
		pthread_mutex_unlock(&(data_found->data_value_lock));
	}

	return data_found != NULL;
}

void remove_data_cashtable(cashtable* cashtable_p, c_data* data_to_del)
{
	unsigned int index = hash_data(data_to_del) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	write_lock(&(bucket->data_list_lock));
	pthread_mutex_lock(&(data_to_del->data_value_lock));
		remove_bucket_data_unsafe(bucket, data_to_del);
	write_unlock(&(bucket->data_list_lock));

		return_used_data_to_manager(&(cashtable_p->data_memory_manager), data_to_del);
	pthread_mutex_lock(&(data_to_del->data_value_lock));
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