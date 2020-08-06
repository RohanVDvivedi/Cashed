#ifndef DATA_H
#define DATA_H

#include<dstring.h>
#include<rwlock.h>

typedef struct data data;
struct data
{
	// this will point to the next bucket of the hashtable
	data* hashtable_next;

	// this lock only protects reading and writing to the 
	// key value pair inside the data
	rwlock data_lock;

	unsigned int key_size;
	unsigned int value_size;

	char key_value[];
	//   key = key_value[0]
	// value = key_value[key_size]
};

// you must take appropriate lock on the data_lock before calling any of the below function that modifies or reads data

unsigned int size_of_data(const dstring* key, const dstring* value);

void init_data(data* data_p, const dstring* key, const dstring* value);

// updates content from value to data->value
void update_value(data* data_p, const dstring* value);

// appends the content of data->key to key
void append_key(const data* data_p, dstring* append_to);

// appends the content of data->key to key
void append_value(const data* data_p, dstring* append_to);

int compare_data(const data* data_p1, const data* data_p2);

unsigned long long int hash_data(const data* data_p);

void deinit_data(data* data_p);

#endif