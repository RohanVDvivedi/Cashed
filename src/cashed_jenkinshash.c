#include<jenkinshash.h>

unsigned long long int jenkins_hash_dstring(const dstring* key)
{
	return jenkins_hash(key->cstring, key->bytes_occupied-1);
}

unsigned long long int jenkins_hash(const void* data_p, unsigned long long int size_in_bytes)
{
	unsigned long long int result = 0;
	result = ((size_in_bytes << 3) % 397);
	result = result | (result << 10);
	result = (((result + size_in_bytes) * 8353) % 8513) + size_in_bytes;
	result = result * (((uint8_t*)(data_p))[0]);
	result = result * (((uint8_t*)(data_p))[size_in_bytes - 2]);
	result = result * 9973;
	return result;
}