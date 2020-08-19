#include<cashed_bucket.h>

#include<cashed_data_class.h>

#include<stdlib.h>

void init_cashed_bucket(cashed_bucket* bucket)
{
	initialize_rwlock(&(bucket->data_list_lock));
	bucket->data_list = NULL;
}

data* find_cashed_bucket_data_by_key_unsafe(cashed_bucket* bucket, const dstring* key, data** prev_return)
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

void insert_cashed_bucket_head_unsafe(cashed_bucket* bucket, data* new_data)
{
	new_data->h_next = bucket->data_list;
	bucket->data_list = new_data;
}

// if prev is NULL, then we remove head
// returns the removed node
data* remove_cashed_bucket_data_next_of_unsafe(cashed_bucket* bucket, data* prev)
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

void deinit_cashed_bucket(cashed_bucket* bucket)
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