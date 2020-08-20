#include<cashed_bucket.h>

#include<cashed_data_class.h>

#include<stdlib.h>

void init_bucket(c_bucket* bucket)
{
	initialize_rwlock(&(bucket->data_list_lock));
	bucket->data_list = NULL;
}

c_data* find_bucket_data_by_key_unsafe(c_bucket* bucket, const dstring* key, c_data** prev_return)
{
	c_data* data_test = bucket->data_list;
	c_data* prev = NULL;
	while(data_test != NULL && compare_key(data_test, key) != 0)
	{
		prev = data_test;
		data_test = data_test->h_next;
	}
	if(prev_return != NULL)
		*prev_return = prev;
	return data_test;
}

void insert_bucket_head_unsafe(c_bucket* bucket, c_data* new_data)
{
	new_data->h_next = bucket->data_list;
	bucket->data_list = new_data;
}

// if prev is NULL, then we remove head
// returns the removed node
c_data* remove_bucket_data_next_of_unsafe(c_bucket* bucket, c_data* prev)
{
	c_data* data_ret = NULL;
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

void deinit_bucket(c_bucket* bucket)
{
	deinitialize_rwlock(&(bucket->data_list_lock));
	c_data* data_from_list = bucket->data_list;
	while(data_from_list != NULL)
	{
		c_data* data_to_delete = data_from_list;
		data_from_list = data_from_list->h_next;
		return_used_data(data_to_delete->data_class, data_to_delete);
	}
	bucket->data_list = NULL;
}