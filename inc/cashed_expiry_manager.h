#ifndef CASHED_EXPIRY_MANAGER_H
#define CASHED_EXPIRY_MANAGER_H

#include<heap.h>

#include<job.h>

#include<cashed_data.h>

#include<pthread.h>

// c_expiry_manager, is a heap of only the elements that would be expired in future
// 

typedef struct c_expiry_manager c_expiry_manager;
struct c_expiry_manager
{
	pthread_mutex_t  expiry_heap_lock;

	// the actual heap of elements
	// it is a min heap based on comparison of the actual expiry times of the elements
	heap expiry_heap;

	job expiry_manager_job;
	pthread_cond_t conditional_wakeup_on_expiry;
};

typedef struct cashtable cashtable;

// allocated memory required for expiry_heap, initialized its lock, 
// and starts long running job, which removed elements from heap when they expire
// as well as goes to sleep, utill the next job expiries or a new element is inserted
void init_expiry_heap(c_expiry_manager* cem, unsigned int min_element_count, cashtable* cashtable_p);

// if the data_p does not have an expiry_seconds, i.e. it it is null,
// it won't be inserted into the heap
void register_data_for_expiry(c_expiry_manager* cem, c_data* data_p);

// data elements, will not be freed, by the expiry manager gets dinitialized
void deinit_expiry_heap(c_expiry_manager* cem);

#endif