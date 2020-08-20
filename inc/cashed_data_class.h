#ifndef CASHED_DATA_CLASS_H
#define CASHED_DATA_CLASS_H

#include<linkedlist.h>
#include<cashed_data.h>

// for fulfilling the request for a new data,
// the cashed data class first serches for a free element that it could readily provide
// if it does not exist, it will create new data 

typedef struct cashed_data_class cashed_data_class;
struct cashed_data_class
{
	// this is the size of each data element that this data class will maintain
	unsigned int total_data_size;

	// class_id and total_data_size are constants throughout the life of any data_class

	// this lock only protects both the lists of this data_class (free_list and used_list, as described below)
	pthread_mutex_t list_locks;

	// this is the linked list of data elements that are currently existing as valid data elements in the cashtable
	// in the used_list, head is the newest data, and tail is the oldest data 
	linkedlist used_list;
	unsigned int used_data_count;

	// this is the linked list of data elements that are free data
	linkedlist free_list;
	unsigned int free_data_count;

	// as the intution suggests from the names of the linkedlists, 
	// the data of size data_size can only exists in any one of the free_list or used_list
};

void init_cashed_data_class(cashed_data_class* cdc, unsigned int total_data_size);

// function to get new data of the size as mentioned by the data_class
data* get_cashed_data(cashed_data_class* cdc);

// when the data has been used, return it to the data_class, so that someone else can find it
void return_used_data(cashed_data_class* cdc, data* free_data);

// bump this data element to the head of the list, so that it is not prioritized to be reclaimed any time sooner
void bump_used_data_on_reuse(cashed_data_class* cdc, data* free_data);

// this function will essentially free up all the memory, hoarded by all the free_data in the free_list
// by calling free on all the data
void release_all_free_data(cashed_data_class* cdc);

// you must return all the data to the data_class,
// then only you should deinit the data_class, else you run into memory leaks
void deinit_cashed_data_class(cashed_data_class* cdc);

#endif