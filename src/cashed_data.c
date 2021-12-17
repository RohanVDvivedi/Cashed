#include<cashed_data.h>

#include<cashed_data_class.h>
#include<cashed_jenkinshash.h>

#include<string.h>

unsigned int get_required_size_of_data(const dstring* key, const dstring* value)
{
	return (sizeof(c_data) + get_char_count_dstring(key) + get_char_count_dstring(value));
}

unsigned int get_total_size_of_data(const c_data* data_p)
{
	return data_p->data_class->total_data_size;
}

void init_data(c_data* data_p, c_data_class* data_class)
{
	data_p->data_class = data_class;

	initialize_llnode(&(data_p->hash_bucket_llnode));
	initialize_llnode(&(data_p->data_class_llnode));

	clock_gettime(CLOCK_REALTIME, &(data_p->set_up_time));
	data_p->expiry_seconds = -1;

	data_p->key_size = 0;
	data_p->value_size = 0;

	pthread_mutex_init(&(data_p->data_value_lock), NULL);
}

void set_data_expiry(c_data* data_p, int expiry_seconds)
{
	clock_gettime(CLOCK_REALTIME, &(data_p->set_up_time));
	data_p->expiry_seconds = ((expiry_seconds <= 0) ? -1 : expiry_seconds);
}


void set_data_key(c_data* data_p, const dstring* key)
{
	data_p->key_size = key->bytes_occupied;
	memcpy(data_p->key_value, get_byte_array_dstring(key), data_p->key_size);
}

void set_data_value(c_data* data_p, const dstring* value)
{
	data_p->value_size = value->bytes_occupied;
	memcpy(data_p->key_value + data_p->key_size, get_byte_array_dstring(value), data_p->value_size);
}

void append_data_key(const c_data* data_p, dstring* append_to)
{
	concatenate_dstring(append_to, &get_literal_dstring(data_p->key_value, data_p->key_size));
}

void append_data_value(const c_data* data_p, dstring* append_to)
{
	concatenate_dstring(append_to, &get_literal_dstring(data_p->key_value + data_p->key_size, data_p->value_size));
}

int compare_data(const c_data* data_p1, const c_data* data_p2)
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

int compare_key(const c_data* data_p1, const dstring* key)
{
	if(data_p1->key_size > get_char_count_dstring(key))
		return 1;
	else if(data_p1->key_size < get_char_count_dstring(key))
		return -1;
	else
		return memcmp(data_p1->key_value, get_byte_array_dstring(key), get_char_count_dstring(key));
}

static int compare_timespecs(struct timespec t1, struct timespec t2)
{
	if(t1.tv_sec > t2.tv_sec)
		return 1;
	else if(t1.tv_sec < t2.tv_sec)
		return -1;
	else
	{
		if(t1.tv_nsec > t2.tv_nsec)
			return 1;
		else if(t1.tv_nsec < t2.tv_nsec)
			return -1;
		else
			return 0;
	}
}

int compare_expiry(const c_data* data_p1, const c_data* data_p2)
{
	if(data_p1->expiry_seconds == -1 && data_p1->expiry_seconds != -1)
		return 1;
	else if(data_p1->expiry_seconds != -1 && data_p1->expiry_seconds == -1)
		return -1;
	else if(data_p1->expiry_seconds == -1 && data_p1->expiry_seconds == -1)
		return 0;
	else
	{
		struct timespec expiry1 = data_p1->set_up_time;
		expiry1.tv_sec += data_p1->expiry_seconds;

		struct timespec expiry2 = data_p2->set_up_time;
		expiry2.tv_sec += data_p2->expiry_seconds;

		return compare_timespecs(expiry1, expiry2);
	}
}

int has_expiry_elapsed(const c_data* data_p)
{
	struct timespec expiry = data_p->set_up_time;
	expiry.tv_sec += data_p->expiry_seconds;

	struct timespec now_time;
	clock_gettime(CLOCK_REALTIME, &now_time);

	return compare_timespecs(now_time, expiry) >= 0;
}

unsigned int hash_data(const c_data* data_p)
{
	return modified_jenkins_hash(data_p->key_value, data_p->key_size);
}

void deinit_data(c_data* data_p)
{
	data_p->key_size = 0;
	data_p->value_size = 0;
	pthread_mutex_destroy(&(data_p->data_value_lock));
}