#include<cashed_data_manager.h>

#include<stdlib.h>

void init_data_manager(c_data_manager* cdcm, unsigned int least_total_data_size, unsigned int total_data_size_increments, unsigned int data_class_count)
{
	cdcm->least_total_data_size = least_total_data_size;
	cdcm->total_data_size_increments = total_data_size_increments;
	cdcm->data_class_count = data_class_count;
	cdcm->data_classes = malloc(sizeof(c_data_class) * cdcm->data_class_count);
	for(int i = 0; i < cdcm->data_class_count; i++)
		init_data_class(cdcm->data_classes + i, cdcm->least_total_data_size + (i * cdcm->total_data_size_increments));
}

static int get_index_from_required_data_size(c_data_manager* cdcm, unsigned int data_size)
{
	int index = 0;
	if(data_size > cdcm->least_total_data_size)
	{
		index = (data_size - cdcm->least_total_data_size) / cdcm->total_data_size_increments;
		if((data_size - cdcm->least_total_data_size) % cdcm->total_data_size_increments)
			index += 1;
	}
	return index;
}

int advise_to_reuse_data(c_data_manager* cdcm, unsigned int total_data_size, unsigned int required_data_size)
{
	int index_old = get_index_from_required_data_size(cdcm, total_data_size);
	int index_new = get_index_from_required_data_size(cdcm, required_data_size);
	return total_data_size > required_data_size && (index_new >= 2 || (index_old - index_new <= 2));
}

c_data* get_cached_data_from_manager(c_data_manager* cdcm, unsigned int required_size)
{
	unsigned int index = get_index_from_required_data_size(cdcm, required_size);
	c_data_class* cdc = cdcm->data_classes + index;
	return get_cached_data(cdc);
}

void return_used_data_to_manager(c_data_manager* cdcm, c_data* free_data)
{
	c_data_class* cdc = free_data->data_class;
	return_used_data(cdc, free_data);
}

void deinit_data_manager(c_data_manager* cdcm)
{
	for(unsigned int i = 0; i < cdcm->data_class_count; i++)
	{
		release_all_free_data(cdcm->data_classes + i);
		deinit_data_class(cdcm->data_classes + i);
	}
}