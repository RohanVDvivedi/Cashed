#ifndef CASHED_BUCKET_H
#define CASHED_BUCKET_H

#include<cashed_data.h>

typedef struct c_bucket c_bucket;
struct c_bucket
{
	// this lock is responsible for protecting
	// data_list pointer of the bucket and 
	// all the h_next pointers in the linked list
	rwlock data_list_lock;

	c_data* data_list;
};

void init_bucket(c_bucket* bucket);

c_data* find_bucket_data_by_key_unsafe(c_bucket* bucket, const dstring* key, c_data** prev_return);

void insert_bucket_head_unsafe(c_bucket* bucket, c_data* new_data);

// if prev is NULL, then we remove head
// returns the removed node
c_data* remove_bucket_data_next_of_unsafe(c_bucket* bucket, c_data* prev);

void deinit_bucket(c_bucket* bucket);

#endif