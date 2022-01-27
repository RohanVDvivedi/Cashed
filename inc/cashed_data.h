#ifndef DATA_H
#define DATA_H

#include<pthread.h>

#include<dstring.h>
#include<linkedlist.h>
#include<heap.h>

#include<time.h>

typedef struct c_data_class c_data_class;

typedef struct c_data c_data;
struct c_data
{
	// this is the cashed data class that this data belongs to
	// this pointer helps define the size of the data element
	// and hence it remains the same through out the life of the data
	c_data_class* data_class;

	// this llnode will be used in the data_list linkedlist of the bucket of the hashtable
	// this llnode will be protected by the corresponding data_list_lock reader writer lock
	// of the corresponding c_bucket struct, that this data belongs to
	llnode hash_bucket_llnode;

	// this llnode will be used in the data_class linkedlist, in lru
	// this llnode will be protected by the corresponding lru_lock mutex in the data_class, that this data belongs to
	llnode data_class_llnode;

	// hpnode in expiry manager
	// It stores the index that refers to the position of this data, in the expiry heap aka expiry_manager
	// this variable is protected by the lock in the expiry manger, while it is working on the expiry heap
	hpnode expiry_manager_hpnode;

	// this lock only protects reading and writing to the key and value of the data, and the setup time and the expiry_seconds (explained below)
	// key remains same throughout the life of the data, and at time you may skip taking lock
	pthread_mutex_t data_value_lock;

	// this is the time when the expiry of this data was last setup
	struct timespec set_up_time;

	// expiry in seconds, the data is suppossed to be deleted/removed from the cashtable, 
	// once expiry_seconds have elapsed from its setup time
	// -1 represents eternal life, every time a data is called for set_key_value or update_value
	// expiry_seconds is set to -1
	int expiry_seconds;

	// the lenth of key and value of this data
	unsigned int key_size;
	unsigned int value_size;

	char key_value[];
	//   key = key_value[0]
	// value = key_value[key_size]
};

// you must take appropriate lock on the data_lock before calling any of the below function that modifies or reads data

unsigned int get_required_size_of_data(const dstring* key, const dstring* value);

unsigned int get_total_size_of_data(const c_data* data_p);

void init_data(c_data* data_p, c_data_class* data_class);

// the below functions allow you to individually set/update key value and expiry
// you must call these functions while holding the data value lock
// set_data_expiry must be called while the data does not exist in the expiry manager heap
void set_data_expiry(c_data* data_p, int expiry_seconds);
void set_data_key(c_data* data_p, const dstring* key);
// set_data_value must be called while the data does not exist in the cashtable, and is free
void set_data_value(c_data* data_p, const dstring* value);

// appends the content of data->key to key
void append_data_key(const c_data* data_p, dstring* append_to);

// appends the content of data->key to key
void append_data_value(const c_data* data_p, dstring* append_to);

// both the below dunction can be used to compare keys for the data
int compare_data(const c_data* data_p1, const c_data* data_p2);
int compare_key(const c_data* data_p1, const dstring* key);

// compare expiry time for data_p1 and data_p2
// returns 1 if data_p1's expiry time is further in future than data_p2,
// returns 0 if they are same, else will return 0
int compare_expiry(const c_data* data_p1, const c_data* data_p2);

// check if the expiry of time has elapsed
int has_expiry_elapsed(const c_data* data_p);

unsigned int hash_data(const c_data* data_p);

void deinit_data(c_data* data_p);

#endif