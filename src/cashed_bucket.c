#include<cashed_bucket.h>

#include<cashed_data_class.h>

#include<stdlib.h>
#include<stddef.h>

void init_bucket(c_bucket* bucket)
{
	initialize_rwlock(&(bucket->data_list_lock));
	initialize_linkedlist(&(bucket->data_list), offsetof(c_data, hash_bucket_llnode));
}

c_data* find_bucket_data_by_key_unsafe(c_bucket* bucket, const dstring* key)
{
	return (c_data*) find_equals_in_list(&(bucket->data_list), key, (int (*)(const void*, const void*))(compare_key));
}

void insert_bucket_head_unsafe(c_bucket* bucket, c_data* new_data)
{
	insert_head(&(bucket->data_list), new_data);
}

void remove_bucket_data_unsafe(c_bucket* bucket, c_data* data_to_remove)
{
	remove_from_list(&(bucket->data_list), data_to_remove);
}

void deinit_bucket(c_bucket* bucket)
{
	deinitialize_rwlock(&(bucket->data_list_lock));
	while(!is_linkedlist_empty(&(bucket->data_list)))
	{
		c_data* data_to_delete = (c_data*) get_head(&(bucket->data_list));
		remove_from_list(&(bucket->data_list), data_to_delete);
		return_used_data(data_to_delete->data_class, data_to_delete);
	}
}