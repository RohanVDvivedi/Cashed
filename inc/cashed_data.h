#ifndef DATA_H
#define DATA_H

#include<pthread.h>

#include<dstring.h>
#include<linkedlist.h>

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

	// this llnode will be used in the data_class linkedlist, either in used_list or free_list
	// this llnode will be protected by the corresponding list_locks mutex in the data_class, that this data belongs to
	llnode data_class_llnode;

	// this is the index that refers to the position of this data, in the expiry heap aka expiry_manager
	// this varibale is protected by the lock in the expiry manger, while it is working on the expiry heap
	unsigned int expiry_heap_manager_index;

	// this lock only protects reading and writing to the key and value of the data, and the setup time and the expiry_seconds (explained below)
	// key remains same throughout the life of the data, and at time you may skip taking lock
	pthread_mutex_t data_value_lock;

	// BELOW are the time structures maintained for each data

	// this is the time when the data was last setup
	// this is caused by either calling set_key_value or update_value
	struct timespec set_up_time;

	// expiry in seconds, the data is suppossed to be deleted/removed from the cashtable, 
	// once expiry_seonds have elapsed from its setup time
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

void set_data_key_value_expiry(c_data* data_p, const dstring* key, const dstring* value, int expiry_seconds);

// updates content from value to data->value
void update_value_expiry(c_data* data_p, const dstring* value, int expiry_seconds);

// appends the content of data->key to key
void append_data_key(const c_data* data_p, dstring* append_to);

// appends the content of data->key to key
void append_data_value(const c_data* data_p, dstring* append_to);

// both the below dunction can be used to compare keys for the data
int compare_data(const c_data* data_p1, const c_data* data_p2);
int compare_key(const c_data* data_p1, const dstring* key);

unsigned long long int hash_data(const c_data* data_p);

void deinit_data(c_data* data_p);

#endif