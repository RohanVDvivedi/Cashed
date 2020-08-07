#ifndef DATA_H
#define DATA_H

#include<dstring.h>
#include<rwlock.h>

typedef struct data data;
struct data
{
	unsigned int total_data_size;

	// this will point to the next bucket of the hashtable
	// this pointer has to be protected by the data_list_lock of the bucket in which this data lives
	data* h_next;

	// this lock only protects reading and writing to the value of the data
	// key remains same throughout the life of the data
	rwlock data_value_lock;

	unsigned int key_size;
	unsigned int value_size;

	char key_value[];
	//   key = key_value[0]
	// value = key_value[key_size]
};

// you must take appropriate lock on the data_lock before calling any of the below function that modifies or reads data

unsigned int size_of_data(const dstring* key, const dstring* value);

void init_data(data* data_p, unsigned int total_data_size, const dstring* key, const dstring* value);

// updates content from value to data->value
void update_value(data* data_p, const dstring* value);

// appends the content of data->key to key
void append_data_key(const data* data_p, dstring* append_to);

// appends the content of data->key to key
void append_data_value(const data* data_p, dstring* append_to);

// both the below dunction can be used to compare keys for the data
int compare_data(const data* data_p1, const data* data_p2);
int compare_key(const data* data_p1, const dstring* key);

unsigned long long int hash_data(const data* data_p);

void deinit_data(data* data_p);

#endif