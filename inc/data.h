#ifndef DATA_H
#define DATA_H

#include<dstring.h>

typedef struct data data;
struct data
{
	// this will point to the next bucket of the hashtable
	data* next_hash_data;

	unsigned int key_size;
	unsigned int value_size;

	char key_value[];
	//   key = key_value[0]
	// value = key_value[key_size]
};

void init_data(data* data_p, const dstring* key, const dstring* value);

void init_dummy_data(data* data_p, char* key);

int compare_data(data* data_p1, data* data_p2);

unsigned int hash_data(data* data_p);

#endif