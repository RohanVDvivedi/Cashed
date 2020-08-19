#ifndef CASHED_BUCKET_H
#define CASHED_BUCKET_H

#include<data.h>

typedef struct cashed_bucket cashed_bucket;
struct cashed_bucket
{
	// this lock is responsible for protecting
	// data_list pointer of the bucket and 
	// all the h_next pointers in the linked list
	rwlock data_list_lock;

	data* data_list;
};

void init_cashed_bucket(cashed_bucket* bucket);

data* find_cashed_bucket_data_by_key_unsafe(cashed_bucket* bucket, const dstring* key, data** prev_return);

void insert_cashed_bucket_head_unsafe(cashed_bucket* bucket, data* new_data);

// if prev is NULL, then we remove head
// returns the removed node
data* remove_cashed_bucket_data_next_of_unsafe(cashed_bucket* bucket, data* prev);

void deinit_cashed_bucket(cashed_bucket* bucket);

#endif