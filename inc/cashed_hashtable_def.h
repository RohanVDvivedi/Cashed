#ifndef CASHED_HASHTABLE_DEF_H
#define CASHED_HASHTABLE_DEF_H

#include<cashed_bucket.h>
#include<cashed_data_manager.h>
#include<cashed_expiry_manager.h>

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
	// every public api operation, on this version of cashtable
	// will use this lock, all other internal locks are disabled, for this branch
	// this is for simplicity and ease of design of the cache
	pthread_mutex_t global_cashtable_lock;

	unsigned int bucket_count;
	c_bucket* buckets;

	c_data_manager data_memory_manager;

	c_expiry_manager expiry_manager;
};

void init_cashtable(cashtable* cashtable_p, unsigned int bucket_count);

#include<cashed_hashtable.h>

void deinit_cashtable(cashtable* cashtable_p);

#endif