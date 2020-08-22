#ifndef CASHED_HASHTABLE_DEF_H
#define CASHED_HASHTABLE_DEF_H

#include<cashed_bucket.h>
#include<cashed_data_manager.h>

/*
** cashtable is indeed a hashtable, cashtable will just help you against name collisions in your code
** also cashtable sounds cool, isn't it? LOL
*/

// hashtable is bound to provide uniqueness using the get, set and del functions

// the data is organized as a hashtable,
// each bucket is a singly linkedlist
// each bucket is protected using a lock

typedef struct cashtable cashtable;
struct cashtable
{
	// this field remains constant, as long as the cashtable is alive
	unsigned int bucket_count;

	// lock used to protect the data count
	pthread_mutex_t data_count_lock;
	unsigned int data_count;
	
	c_bucket* buckets;

	c_data_manager data_memory_manager;
};

void init_cashtable(cashtable* cashtable_p, unsigned int bucket_count);

#include<cashed_hashtable.h>

void remove_data_cashtable(cashtable* cashtable_p, c_data* data_to_del);

void deinit_cashtable(cashtable* cashtable_p);

#endif