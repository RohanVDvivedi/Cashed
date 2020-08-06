#ifndef CASHED_HASHTABLE_H
#define CASHED_HASHTABLE_H

#include<data.h>
#include<rwlock.h>

// hashtable is bound to provide uniqueness using the get, set and del functions

// the data is organized a hashtable,
// each bucket is a singly linkedlist
// each bucket is protected using a lock

typedef struct hashtable hashtable;
typedef struct hashbucket hashbucket;
struct hashtable
{
	// this field remains constant, as long as the hashtable is alive
	unsigned int bucket_count;

	// lock used to protect the data count
	pthread_mutex_t data_count_lock;
	unsigned int data_count;
	
	hashbucket* hashbuckets;
};

struct hashbucket
{
	// this lock is responsible for protecting
	// data_list pointer of the bucket and 
	// all the hashtable_next_data pointers in the linked list
	rwlock data_list_lock;

	data* data_list;
};

void init_hashtable(hashtable* hashtable_p, unsigned int bucket_count);

// returns 0, if the data was not found
// the contents of the value will be appended to the return_value dstring
int get_hashtable(hashtable* hashtable_p, const dstring* key, dstring* return_value);

// a key value is inserted or updated by this call
// the contents of you key and value will not be used in the dstring and hence you may delete them after thic call 
// returns 1 if it was an insert, else it returns 0 for an update
int set_hashtable(hashtable* hashtable_p, const dstring* key, const dstring* value);

// returns 0, if the no data was found to delete by the given key
int del_hashtable(hashtable* hashtable_p, const dstring* key);

void deinit_hashtable(hashtable* hashtable_p, unsigned int bucket_count);

#endif