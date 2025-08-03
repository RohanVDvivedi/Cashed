#ifndef CASHED_DATA_H
#define CASHED_DATA_H

#include<cutlery/cachemap.h>
#include<cutlery/pheap.h>

#include<stdint.h>

typedef struct cashed_data cashed_data;
struct cashed_data
{
	// embedded node for the hashtable in cashed_hashtable
	cchnode embed_node1;

	// embedded node for the expiryheap in cashed_hashtable
	phpnode embed_node2;

	// absolute epoch (in microseconds) when to expiry this particular value
	uint64_t expiry_in_microseconds;

	// bytes in payload representing the key
	cy_uint key_size;

	// bytes in payload representing the value (placed right after the key)
	cy_uint value_size;

	// total size of the payload (redundant information)
	cy_uint payload_size;

	char payload[];
};

#endif