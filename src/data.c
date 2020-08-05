#include<data.h>

#include<jenkinshash.h>

unsigned int size_of_data(const dstring* key, const dstring* value)
{
	return (sizeof(data) + (key->bytes_occupied - 1) + (value->bytes_occupied - 1));
}

void init_data(data* data_p, const dstring* key, const dstring* value)
{
	data_p->hashtable_next = NULL;

	initialize_rwlock(&(data_p->data_lock));

	data_p->key_size = key->bytes_occupied - 1;
	memcpy(data_p->key_value, key->cstring, data_p->key_size);

	data_p->value_size = value->bytes_occupied - 1;
	memcpy(data_p->key_value + data_p->key_size, value->cstring, data_p->value_size);
}

void update_value(data* data_p, const dstring* value)
{
	data_p->value_size = value->bytes_occupied - 1;
	memcpy(data_p->key_value + data_p->key_size, value->cstring, data_p->value_size);
}

void append_data_key(const data* data_p, dstring* append_to)
{
	appendn_to_dstring(append_to, (char*)(data_p->key_value), data_p->key_size);
}

void append_data_value(const data* data_p, dstring* append_to)
{
	appendn_to_dstring(append_to, (char*)(data_p->key_value + data_p->key_size), data_p->value_size);
}

void init_dummy_data(data* data_p, char* key)
{
	data_p->key_size = strlen(key);
	memcpy(data_p->key_value, key, data_p->key_size);
}

int compare_data(const data* data_p1, const data* data_p2)
{
	if(data_p1 == data_p2)
		return 0;
	else if(data_p1->key_size > data_p2->key_size)
		return 1;
	else if(data_p1->key_size < data_p2->key_size)
		return -1;
	else
		return memcmp(data_p1->key_value, data_p2->key_value, data_p2->key_size);
}

unsigned int hash_data(const data* data_p)
{
	return jenkins_hash(data_p->key_value, data_p->key_size);
}

void deinit_data(data* data_p)
{
	deinitialize_rwlock(&(data_p->data_lock));
}