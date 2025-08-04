#include<cashed/cashed_data.h>

#include<cutlery/cutlery_math.h>

cy_uint hash_cashed_data_using_key(const void* data)
{
	const cashed_data* d = data;

	uint64_t hash = 0xcbf29ce484222325ULL; // FNV-1a 64-bit offset basis
	const uint64_t prime = 0x100000001b3ULL; // FNV-1a 64-bit prime
	for(cy_uint i = 0; i < d->key_size; i++)
	{
		hash ^= ((uint64_t)(d->payload[i]));
		hash *= prime;
	}

	return hash;
}

int compare_cashed_data_using_key(const void* data1, const void* data2)
{
	const cashed_data* d1 = data1;
	const cashed_data* d2 = data2;

	int cmp = memory_compare(d1->payload, d2->payload, min(d1->key_size, d2->key_size));
	if(cmp)
		return cmp;

	// one if them is prefix of another, now compare them by size
	return compare_numbers(d1->key_size, d2->key_size);
}

int compare_cashed_data_using_expiry(const void* data1, const void* data2)
{
	const cashed_data* d1 = data1;
	const cashed_data* d2 = data2;

	return compare_numbers(d1->absolute_expiry_in_microseconds, d2->absolute_expiry_in_microseconds);
}

void initialize_cashed_data(cashed_data* data, const dstring* key, const dstring* value, uint64_t absolute_expiry_in_microseconds)
{
	// initialize embedded nodes
	initialize_cchnode(&(data->embed_node1));
	initialize_phpnode(&(data->embed_node2));

	// set absolute expiry in microsecond, this is the exactly absolute microsecond in CLOCK_MONOTONIC when this object must cease to exist
	data->absolute_expiry_in_microseconds = absolute_expiry_in_microseconds;

	// set key in the cashed_data
	memory_move(data->payload, get_byte_array_dstring(key), get_char_count_dstring(key));
	data->key_size = get_char_count_dstring(key);

	// set value in the cashed_data
	memory_move(data->payload + data->key_size, get_byte_array_dstring(value), get_char_count_dstring(value));
	data->value_size = get_char_count_dstring(value);
}