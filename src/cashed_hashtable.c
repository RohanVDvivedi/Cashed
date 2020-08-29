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
	cashtable_p->buckets = calloc(cashtable_p->bucket_count, sizeof(c_bucket));
	for(unsigned int i = 0; i < cashtable_p->bucket_count; i++)
		init_bucket(cashtable_p->buckets + i);

	// create data memory manager for the cashtable
	init_data_manager(&(cashtable_p->data_memory_manager), 3 * sizeof(c_data), 30, 35);

	// initialize the cashed expiry manager
	init_expiry_heap(&(cashtable_p->expiry_manager), bucket_count, cashtable_p);

	// initialize global lock
	pthread_mutex_init(&(cashtable_p->global_cashtable_lock), NULL);
}

int get_value_cashtable(cashtable* cashtable_p, const dstring* key, dstring* return_value)
{
	pthread_mutex_lock(&(cashtable_p->global_cashtable_lock));

	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key);

	if(data_found != NULL)
	{
		append_data_value(data_found, return_value);
		bump_used_data_on_reuse_unsafe(data_found->data_class, data_found);
	}

	pthread_mutex_unlock(&(cashtable_p->global_cashtable_lock));

	return data_found != NULL;
}

int set_key_value_expiry_cashtable(cashtable* cashtable_p, const dstring* key, const dstring* value, int expiry_seconds)
{
	pthread_mutex_lock(&(cashtable_p->global_cashtable_lock));

	unsigned int size_of_new_data = get_required_size_of_data(key, value);

	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key);
		
	int new_allocation_and_insertion_required = 0;
	// this variable lets us know, if we must insert a new data with the corresponding key and value

	// if a data with the same key value is found, we check if we could reuse the memory 
	// we wll try and reuse if possible
	if(data_found != NULL)
	{
		// if this found data is set to expire in future, then we must register it
		if(data_found->expiry_seconds != -1)
			de_register_data_from_expiry_heap_unsafe(&(cashtable_p->expiry_manager), data_found);

		// check with memory manager, if it could allow us to reuse the data, for new value to the same key
		if(advise_to_reuse_data(&(cashtable_p->data_memory_manager), get_total_size_of_data(data_found), size_of_new_data))
		{
			update_value_expiry(data_found, value, expiry_seconds);
			bump_used_data_on_reuse_unsafe(data_found->data_class, data_found);
			if(data_found->expiry_seconds != -1)
				register_data_for_expiry_unsafe(&(cashtable_p->expiry_manager), data_found);
		}
		else
		{
			remove_bucket_data_unsafe(bucket, data_found);
			return_used_data_unsafe(data_found->data_class, data_found);
			new_allocation_and_insertion_required = 1;
		}
	}
	else
		new_allocation_and_insertion_required = 1;

	if(new_allocation_and_insertion_required)
	{
		c_data_class* data_class_for_new_data = get_managed_data_class_by_size(&(cashtable_p->data_memory_manager), size_of_new_data);
		c_data* new_data = get_cached_data_unsafe(data_class_for_new_data);
		insert_bucket_head_unsafe(bucket, new_data);
		set_data_key_value_expiry(new_data, key, value, expiry_seconds);
		if(data_found->expiry_seconds != -1)
			register_data_for_expiry_unsafe(&(cashtable_p->expiry_manager), data_found);
	}

	pthread_mutex_unlock(&(cashtable_p->global_cashtable_lock));

	return 1;
}

int del_key_value_cashtable(cashtable* cashtable_p, const dstring* key)
{
	pthread_mutex_lock(&(cashtable_p->global_cashtable_lock));

	unsigned int index = jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key);
	if(data_found != NULL)
	{
		remove_bucket_data_unsafe(bucket, data_found);
		de_register_data_from_expiry_heap_unsafe(&(cashtable_p->expiry_manager), data_found);
		return_used_data_unsafe(data_found->data_class, data_found);
	}

	pthread_mutex_unlock(&(cashtable_p->global_cashtable_lock));

	return data_found != NULL;
}

void remove_data_cashtable_unsafe(cashtable* cashtable_p, c_data* data_to_del)
{
	unsigned int index = hash_data(data_to_del) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	remove_bucket_data_unsafe(bucket, data_to_del);
	de_register_data_from_expiry_heap_unsafe(&(cashtable_p->expiry_manager), data_to_del);
	return_used_data_unsafe(data_to_del->data_class, data_to_del);
}

void deinit_cashtable(cashtable* cashtable_p)
{
	pthread_mutex_destroy(&(cashtable_p->global_cashtable_lock));

	for(unsigned int i = 0; i < cashtable_p->bucket_count; i++)
		deinit_bucket(cashtable_p->buckets + i);
	free(cashtable_p->buckets);
	cashtable_p->buckets = NULL;

	deinit_data_manager(&(cashtable_p->data_memory_manager));

	deinit_expiry_heap(&(cashtable_p->expiry_manager));
}

void delete_cashtable(cashtable* cashtable_p)
{
	deinit_cashtable(cashtable_p);
	free(cashtable_p);
}