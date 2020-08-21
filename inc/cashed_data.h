#ifndef DATA_H
#define DATA_H

#include<pthread.h>

#include<dstring.h>
#include<linkedlist.h>

typedef struct c_data_class c_data_class;

typedef struct c_data c_data;
struct c_data
{
	// this is the cashed data class that this data belongs to
	// this pointer helps define the size of the data element
	// and hence it remains the same through out the life of the data
	c_data_class* data_class;

	// this will point to the next bucket of the hashtable
	// this pointer has to be protected by the data_list_lock of the bucket in which this data lives
	c_data* h_next;

	// llnode will be used in the data_class linkedlist, either in used_list or free_list
	// this llnode will be protected by the corresponding list_locks mutex in the data_class, that this data belongs to
	llnode data_class_llnode;

	// this lock only protects reading and writing to the value of the data
	// key remains same throughout the life of the data
	pthread_mutex_t data_value_lock;

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

void set_data_key_value(c_data* data_p, const dstring* key, const dstring* value);

// updates content from value to data->value
void update_value(c_data* data_p, const dstring* value);

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