#ifndef CASHED_BUCKET_H
#define CASHED_BUCKET_H

#include<rwlock.h>

#include<linkedlist.h>

#include<cashed_data.h>

typedef struct c_bucket c_bucket;
struct c_bucket
{
	// this lock is responsible for protecting
	// data_list pointer of the bucket and 
	// all the h_next pointers in the linked list
	rwlock data_list_lock;

	linkedlist data_list;
};

void init_bucket(c_bucket* bucket);

c_data* find_bucket_data_by_key_unsafe(c_bucket* bucket, const dstring* key);

void insert_bucket_head_unsafe(c_bucket* bucket, c_data* new_data);

// if prev is NULL, then we remove head
// returns the removed node
void remove_bucket_data_unsafe(c_bucket* bucket, c_data* data_to_remove);

void deinit_bucket(c_bucket* bucket);

#endif