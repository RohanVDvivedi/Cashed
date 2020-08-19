#include<cashed_data_class_manager.h>

#include<stdlib.h>

void init_cashed_data_class_manager(cashed_data_class_manager* cdcm, unsigned int least_total_data_size, unsigned int total_data_size_increments, unsigned int data_class_count)
{
	cdcm->least_total_data_size = least_total_data_size;
	cdcm->total_data_size_increments = total_data_size_increments;
	cdcm->data_class_count = data_class_count;
	cdcm->data_classes = malloc(sizeof(cashed_data_class) * cdcm->data_class_count);
	for(unsigned int i = 0; i < cdcm->data_class_count; i++)
		init_cashed_data_class(cdcm->data_classes + i, i, cdcm->least_total_data_size + (i * cdcm->total_data_size_increments));
}

data* get_cashed_data_from_manager(cashed_data_class_manager* cdcm, unsigned int required_size)
{
	unsigned int class_id = 0;
	if(required_size > cdcm->least_total_data_size)
	{
		class_id = (required_size - cdcm->least_total_data_size) / cdcm->total_data_size_increments;
		if((required_size - cdcm->least_total_data_size) % cdcm->total_data_size_increments)
		{
			class_id += 1;
		}
	}
	cashed_data_class* cdc = cdcm->data_classes + class_id;
	return get_cashed_data(cdc);
}

void return_used_data_to_manager(cashed_data_class_manager* cdcm, data* free_data)
{
	cashed_data_class* cdc = cdcm->data_classes + free_data->class_id;
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