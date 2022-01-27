#ifndef CASHED_DATA_CLASS_H
#define CASHED_DATA_CLASS_H

#include<linkedlist.h>
#include<bst.h>

#include<cashed_data.h>

#include<cache.h>

// for fulfilling the request for a new data,
// the cashed data class first serches for a free element that it could readily provide
// if it does not exist, it will create new data 

typedef struct c_data_class c_data_class;
struct c_data_class
{
	// this is the size of each data element that this data class will maintain
	unsigned int total_data_size;

	// class_id and total_data_size are constants throughout the life of any data_class

	// this lock only protects lru linkedlist of this data_class
	pthread_mutex_t lru_lock;

	// this is the linked list of data elements that are currently existing as valid data elements in the cashtable
	// in the used_list, head is the newest data (i.e. more recently used data), and tail is the oldest data 
	linkedlist lru;

	// the cache slab allocator
	cache slab_allocator_cache;

	// as the intution suggests from the names of the linkedlists, 
	// the data of size data_size can only exists in any one of the free_list or used_list

	// this is the bst node that will be used, for the binary search tree in the data_manager struct
	// every data_class is compared and stored on their total_data_size attirbute
	bstnode data_manager_bstnode;
};

void init_data_class(c_data_class* cdc, unsigned int total_data_size, unsigned int slab_size, unsigned int max_memory_usage);

// function to get new data of the size as mentioned by the data_class
c_data* allocate_data(c_data_class* cdc, int* needs_eviction);

// return used data, only if you hold lock on the data, this ensures that the data was still in the hash table
// while the data_class moved it from used_list to free_list
// when the data has been used, return it to the data_class, so that someone else can find it
void deallocate_data(c_data_class* cdc, c_data* free_data);

// bump your data, only if you hold lock on the data, this ensures that the data is still in the hash table
// bump this data element to the head of the list, so that it is not prioritized to be reclaimed any time sooner
void bump_used_data_on_reuse(c_data_class* cdc, c_data* used_data);

// you must return all the data to the data_class,
// then only you should deinit the data_class, else you run into memory leaks
void deinit_data_class(c_data_class* cdc);

// to use for comparing and finding correct data_class suitable for your c_data
// use this compare function
int compare_data_classes_on_total_data_size(const c_data_class* cdc1, const c_data_class* cdc2);

#endif