#ifndef CASHED_DATA_CLASS_MANAGER_H
#define CASHED_DATA_CLASS_MANAGER_H

#include<cashed_data_class.h>

// each cashed data_class manager manages data sizes
// from least_total_data_size to least_total_data_size + (data_class_count-1) * total_data_size_increments
// in total of data_class_count number of classes

typedef struct  c_data_manager c_data_manager;
struct c_data_manager
{
	unsigned int least_total_data_size;

	unsigned int total_data_size_increments;

	// total number of data_classes
	// this data_class manages
	unsigned int data_class_count;

	c_data_class* data_classes;
};

void init_data_manager(c_data_manager* cdcm, unsigned int least_total_data_size, unsigned int total_data_size_increments, unsigned int data_class_count);

c_data* get_cached_data_from_manager(c_data_manager* cdcm, unsigned int required_size);

int advise_to_reuse_data(c_data_manager* cdcm, unsigned int total_data_size, unsigned int required_data_size);

void return_used_data_to_manager(c_data_manager* cdcm, c_data* free_data);

void deinit_data_manager(c_data_manager* cdcm);

#endif