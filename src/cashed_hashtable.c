#include<cashed_hashtable.h>

void init_hashbucket(hashbucket* bucket)
{
	initialize_rwlock(&(bucket->data_list_lock));
	bucket->data_list = NULL;
}

void deinit_hashbucket(hashbucket* bucket)
{
	deinitialize_rwlock(&(bucket->data_list_lock));
	bucket->data_list = NULL;
}

void init_hashtable(hashtable* hashtable_p, unsigned int bucket_count)
{
	hashtable_p->bucket_count = bucket_count;
	pthread_mutex_init(&(hashtable_p->data_count_lock), NULL);
	hashtable_p->data_count = 0;
	hashtable_p->hashbuckets = calloc(hashtable_p->bucket_count, sizeof(hashbucket));
	for(unsigned int i = 0; i < hashtable_p->bucket_count; i++)
		init_hashbucket(hashtable_p->hashbuckets + i);
}

int get_hashtable(hashtable* hashtable_p, const dstring* key, dstring* return_value)
{
	
	return 0;
}

int set_hashtable(hashtable* hashtable_p, const dstring* key, const dstring* value)
{
	return 0;
}

int del_hashtable(hashtable* hashtable_p, const dstring* key)
{
	return 0;
}

void deinit_hashtable(hashtable* hashtable_p, unsigned int bucket_count)
{
	pthread_mutex_destroy(&(hashtable_p->data_count_lock));
	if(hashtable_p->hashbuckets != NULL)
	{
		for(unsigned int i = 0; i < hashtable_p->bucket_count; i++)
			deinit_hashbucket(hashtable_p->hashbuckets + i);
		free(hashtable_p->hashbuckets);
		hashtable_p->hashbuckets = NULL;
	}
}