#ifndef CASHED_DATA_CLASS_MANAGER_H
#define CASHED_DATA_CLASS_MANAGER_H

#include<cashed_data_class.h>

#define PAGE_SIZE 4096

// each cashed data_class manager manages data sizes
// from least_total_data_size to least_total_data_size + (data_class_count-1) * total_data_size_increments
// in total of data_class_count number of classes

typedef struct  c_data_manager c_data_manager;
struct c_data_manager
{
	unsigned int least_total_data_size;

	unsigned int total_data_size_increment_percents;

	// total number of data_classes
	// this data_class manages
	unsigned int data_class_count;

	// this is binary search tree, for managing all the data classes ordered on their total_data_size
	bst data_classes;
};

void init_data_manager(c_data_manager* cdcm, unsigned int least_total_data_size, unsigned int total_data_size_increment_percents, unsigned int data_class_count, unsigned int pages_per_slab, unsigned int max_memory_usage);

c_data_class* get_managed_data_class_by_size(c_data_manager* cdcm, unsigned int required_data_size);

int advise_to_reuse_data(c_data_manager* cdcm, unsigned int total_data_size, unsigned int required_data_size);

void deinit_data_manager(c_data_manager* cdcm);

#endif