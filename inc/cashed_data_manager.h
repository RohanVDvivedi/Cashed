#ifndef CASHED_DATA_CLASS_MANAGER_H
#define CASHED_DATA_CLASS_MANAGER_H

#include<cashed_data_class.h>

// each cashed data_class manages data sizes
// from least_total_data_size to least_total_data_size + (data_class_count-1) * total_data_size_increments
// in total of data_class_count number of classes

typedef struct  cashed_data_class_manager cashed_data_class_manager;
struct cashed_data_class_manager
{
	unsigned int least_total_data_size;

	unsigned int total_data_size_increments;

	// total number of data_classes
	// this data_class manages
	unsigned int data_class_count;

	cashed_data_class* data_classes;
};

void init_cashed_data_class_manager(cashed_data_class_manager* cdcm, unsigned int least_total_data_size, unsigned int total_data_size_increments, unsigned int data_class_count);

data* get_cashed_data_from_manager(cashed_data_class_manager* cdcm, unsigned int required_size);

void return_used_data_to_manager(cashed_data_class_manager* cdcm, data* free_data);

void deinit_cashed_data_class_manager(cashed_data_class_manager* cdcm);

#endif