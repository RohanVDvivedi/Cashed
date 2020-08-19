#ifndef CASHED_DATA_CLASS_H
#define CASHED_DATA_CLASS_H

#include<linkedlist.h>

// for fulfilling the request for a new data,
// the cashed data class first serches for a free element that it could readily provide
// if it does not exist, it will create new data 

typedef struct cashed_data_class cashed_data_class;
struct cashed_data_class
{
	// the class_id represents the class_id of data that this data class handles
	// this is a unique identifier, it will be different for every different data class
	unsigned int class_id;

	// this is the size of each data element that this data class will maintain
	unsigned int data_total_size;

	// this is the linked list of data elements that are currently existing as valid data elements in the cashtable
	linkedlist used_list;
	unsigned int used_data_count;

	// this is the linked list of data elements that are free data
	linkedlist free_list;
	unsigned int free_data_count;
};

#endif