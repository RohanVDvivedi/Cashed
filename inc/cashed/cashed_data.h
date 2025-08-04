#ifndef CASHED_DATA_H
#define CASHED_DATA_H

#include<cutlery/cachemap.h>
#include<cutlery/pheap.h>

#include<stdint.h>

typedef struct cashed_data cashed_data;
struct cashed_data
{
	// embedded node for the lruhashtable in cashed_hashtable
	cchnode embed_node1;

	// embedded node for the expiryheap in cashed_hashtable
	phpnode embed_node2;

	// absolute epoch (in microseconds) when to expiry this particular value
	// set to BLOCKING if it is not to be expired
	uint64_t absolute_expiry_in_microseconds;

	// bytes in payload representing the key
	cy_uint key_size;

	// bytes in payload representing the value (placed right after the key)
	cy_uint value_size;

	// total bytes allocated for the payload
	cy_uint payload_capacity;

	char payload[];
	//   key @ &(payload[0])
	// value @ &(payload[key_size])
};

cy_uint hash_cashed_data_using_key(const void* data);

int compare_cashed_data_using_key(const void* data1, const void* data2);

// data which gets expired/evicted first is the smaller one
int compare_cashed_data_using_expiry(const void* data1, const void* data2);

void initialize_cashed_data(cashed_data* data, const dstring* key, const dstring* value, uint64_t absolute_expiry_in_microseconds);

#endif