#include<data.h>

#include<jenkinshash.h>

unsigned int size_of_data(const dstring* key, const dstring* value)
{
	return (sizeof(data) + (key->bytes_occupied - 1) + (value->bytes_occupied - 1));
}

void init_data(data* data_p, const dstring* key, const dstring* value)
{
	data_p->next_hash_data = NULL;

	data_p->key_size = key->bytes_occupied - 1;
	data_p->value_size = value->bytes_occupied - 1;

	memcpy(data_p->key_value, key->cstring, data_p->key_size);
	memcpy(data_p->key_value + data_p->key_size, value->cstring, data_p->value_size);
}

void init_dummy_data(data* data_p, char* key)
{
	data_p->key_size = strlen(key);
	memcpy(data_p->key_value, key, data_p->key_size);
}

int compare_data(data* data_p1, data* data_p2)
{
	if(data_p1->key_size > data_p2->key_size)
		return 1;
	else if(data_p1->key_size < data_p2->key_size)
		return -1;
	else
		return memcmp(data_p1->key_value, data_p2->key_value, data_p2->key_size);
}

unsigned int hash_data(data* data_p)
{
	return jenkins_hash(data_p->key_value, data_p->key_size);
}