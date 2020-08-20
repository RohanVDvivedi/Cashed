#include<cashed_data_manager.h>

#include<stdlib.h>

void init_cashed_data_class_manager(cashed_data_class_manager* cdcm, unsigned int least_total_data_size, unsigned int total_data_size_increments, unsigned int data_class_count)
{
	cdcm->least_total_data_size = least_total_data_size;
	cdcm->total_data_size_increments = total_data_size_increments;
	cdcm->data_class_count = data_class_count;
	cdcm->data_classes = malloc(sizeof(cashed_data_class) * cdcm->data_class_count);
	for(int i = 0; i < cdcm->data_class_count; i++)
		init_cashed_data_class(cdcm->data_classes + i, cdcm->least_total_data_size + (i * cdcm->total_data_size_increments));
}

static int get_index_from_required_data_size(cashed_data_class_manager* cdcm, unsigned int data_size)
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

c_data* get_cashed_data_from_manager(cashed_data_class_manager* cdcm, unsigned int required_size)
{
	unsigned int index = get_index_from_required_data_size(cdcm, required_size);
	cashed_data_class* cdc = cdcm->data_classes + index;
	return get_cashed_data(cdc);
}

void return_used_data_to_manager(cashed_data_class_manager* cdcm, c_data* free_data)
{
	cashed_data_class* cdc = free_data->data_class;
	return_used_data(cdc, free_data);
}

void deinit_cashed_data_class_manager(cashed_data_class_manager* cdcm)
{
	for(unsigned int i = 0; i < cdcm->data_class_count; i++)
	{
		release_all_free_data(cdcm->data_classes + i);
		deinit_cashed_data_class(cdcm->data_classes + i);
	}
}