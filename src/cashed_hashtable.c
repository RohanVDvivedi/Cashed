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
}

int get_value_cashtable(cashtable* cashtable_p, const dstring* key, dstring* return_value)
{
	unsigned int index = modified_jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	read_lock(&(bucket->data_list_lock));

	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key);

	if(data_found != NULL)
	{
		bump_used_data_on_reuse(data_found->data_class, data_found);
		pthread_mutex_lock(&(data_found->data_value_lock));
	}

	read_unlock(&(bucket->data_list_lock));

	if(data_found != NULL)
	{
		append_data_value(data_found, return_value);
		pthread_mutex_unlock(&(data_found->data_value_lock));
	}

	return data_found != NULL;
}

int set_key_value_expiry_cashtable(cashtable* cashtable_p, const dstring* key, const dstring* value, int expiry_seconds)
{
	unsigned int size_of_new_data = get_required_size_of_data(key, value);

	unsigned int index = modified_jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	write_lock(&(bucket->data_list_lock));

	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key);

	// if a data with the same key value is found, we check if we could reuse the memory 
	// we wll try and reuse if possible
	if(data_found != NULL)
	{
		// check with memory manager, if it could allow us to reuse the data, for new value to the same key
		if(advise_to_reuse_data(&(cashtable_p->data_memory_manager), get_total_size_of_data(data_found), size_of_new_data))
		{
			// if this found data is set to expire in future, then we must deregister it from expiry manager
			if(data_found->expiry_seconds != -1)
				de_register_data_from_expiry_heap(&(cashtable_p->expiry_manager), data_found);

			bump_used_data_on_reuse(data_found->data_class, data_found);

			pthread_mutex_lock(&(data_found->data_value_lock));

			set_data_expiry(data_found, expiry_seconds);
			if(data_found->expiry_seconds != -1)
				register_data_for_expiry(&(cashtable_p->expiry_manager), data_found);

			write_unlock(&(bucket->data_list_lock));

				set_data_value(data_found, value);
			pthread_mutex_unlock(&(data_found->data_value_lock));

			// value updated, hence we return
			return 1;
		}

		// remove old value
		else
		{
			if(data_found->expiry_seconds != -1)
				de_register_data_from_expiry_heap(&(cashtable_p->expiry_manager), data_found);
			remove_bucket_data_unsafe(bucket, data_found);
			return_used_data(data_found->data_class, data_found);
		}
	}

	// insert new data

	c_data_class* data_class_for_new_data = get_managed_data_class_by_size(&(cashtable_p->data_memory_manager), size_of_new_data);
	c_data* new_data = get_cached_data(data_class_for_new_data);
	insert_bucket_head_unsafe(bucket, new_data);

	pthread_mutex_lock(&(new_data->data_value_lock));

		set_data_expiry(new_data, expiry_seconds);
		if(new_data->expiry_seconds != -1)
			register_data_for_expiry(&(cashtable_p->expiry_manager), new_data);

	write_unlock(&(bucket->data_list_lock));

		set_data_key(new_data, key);
		set_data_value(new_data, value);
	pthread_mutex_unlock(&(new_data->data_value_lock));

	return 1;
}

int del_key_value_cashtable(cashtable* cashtable_p, const dstring* key)
{
	unsigned int index = modified_jenkins_hash_dstring(key) % cashtable_p->bucket_count;
	c_bucket* bucket = cashtable_p->buckets + index;

	write_lock(&(bucket->data_list_lock));

	c_data* data_found = find_bucket_data_by_key_unsafe(bucket, key);

	if(data_found != NULL)
	{
		if(data_found->expiry_seconds != -1)
			de_register_data_from_expiry_heap(&(cashtable_p->expiry_manager), data_found);

		remove_bucket_data_unsafe(bucket, data_found);

		return_used_data(data_found->data_class, data_found);
	}

	write_unlock(&(bucket->data_list_lock));

	return data_found != NULL;
}

void deinit_cashtable(cashtable* cashtable_p)
{
	deinit_expiry_heap(&(cashtable_p->expiry_manager));
	
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