#ifndef DATA_H
#define DATA_H

typedef struct data data;
struct data
{
	unsigned int hash_of_key;

	unsigned int key_size;
	unsigned int value_size;

	char key_value[];
	//   key = key_value[0]
	// value = key_value[key_size]
};

#endif