#include<cashed_data_manager.h>

#include<stdlib.h>
#include<math.h>
#include<stddef.h>

void init_data_manager(c_data_manager* cdcm, unsigned int least_total_data_size, unsigned int total_data_size_increment_percents, unsigned int data_class_count)
{
	cdcm->least_total_data_size = least_total_data_size;
	cdcm->total_data_size_increment_percents = total_data_size_increment_percents;
	cdcm->data_class_count = data_class_count;
	initialize_bst(&(cdcm->data_classes), AVL_TREE, offsetof(c_data_class, data_manager_bstnode), (int (*)(const void*,const void*))compare_data_classes_on_total_data_size);
	unsigned int total_data_size = least_total_data_size;
	for(int i = 0; i < cdcm->data_class_count; i++)
	{
		c_data_class* data_class = malloc(sizeof(c_data_class));
		init_data_class(data_class, total_data_size);
		insert_in_bst(&(cdcm->data_classes), data_class);
		total_data_size += (unsigned int) ( ((float)(total_data_size)) * ((float)(total_data_size_increment_percents)) / (100.0) );
	}
}

int advise_to_reuse_data(c_data_manager* cdcm, unsigned int total_data_size, unsigned int required_data_size)
{
	if(total_data_size < required_data_size)
		return 0;
	double increment_factor = 1.0 + (((float)cdcm->total_data_size_increment_percents)/100.0);
	double min_total_data_size = cdcm->least_total_data_size;
	double old_index = log(((float)total_data_size)/min_total_data_size)/log(increment_factor);
	double new_index = log(((float)required_data_size)/min_total_data_size)/log(increment_factor);
	return (2.0 >= new_index) || (old_index - new_index <= 2.0);
}

c_data* get_cached_data_from_manager(c_data_manager* cdcm, unsigned int required_size)
{
	c_data_class* cdc = (c_data_class*) find_succeeding_or_equals(&(cdcm->data_classes), &((c_data_class){.total_data_size = required_size}));
	return get_cached_data(cdc);
}

void return_used_data_to_manager(c_data_manager* cdcm, c_data* free_data)
{
	c_data_class* cdc = free_data->data_class;
	return_used_data(cdc, free_data);
}

static void delete_data_class_wrapper(const void* data_class_vp, const void* additional_params)
{
	c_data_class* data_class = (c_data_class*) data_class_vp;
	deinit_data_class(data_class);
	free(data_class);
}

void deinit_data_manager(c_data_manager* cdcm)
{
	for_each_in_bst(&(cdcm->data_classes), POST_ORDER, delete_data_class_wrapper, NULL);
}