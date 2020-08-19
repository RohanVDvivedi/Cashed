#include<cashed_data_class.h>

#include<stdlib.h>
#include<stddef.h>

void init_cashed_data_class(cashed_data_class* cdc, unsigned int class_id, unsigned int data_total_size)
{
	cdc->class_id = class_id;
	cdc->data_total_size = data_total_size;
	pthread_mutex_init(&(cdc->list_locks), NULL);
	cdc->used_data_count = 0;
	initialize_linkedlist(&(cdc->used_list), offsetof(data, data_class_llnode));
	cdc->free_data_count = 0;
	initialize_linkedlist(&(cdc->free_list), offsetof(data, data_class_llnode));
}

data* get_cashed_data(cashed_data_class* cdc)
{
	data* new_data = NULL;
	pthread_mutex_lock(&(cdc->list_locks));
		if(cdc->free_data_count > 0)
		{
			new_data = (data*) get_head(&(cdc->free_list));
			cdc->free_data_count -= remove_head(&(cdc->free_list));
		}
		else
		{
			new_data = malloc(cdc->data_total_size);
			init_data(new_data, cdc->class_id, cdc->data_total_size);
		}
		if(new_data != NULL)
			cdc->used_data_count += insert_head(&(cdc->used_list), new_data);
	pthread_mutex_unlock(&(cdc->list_locks));
	return new_data;
}

void return_used_data(cashed_data_class* cdc, data* free_data)
{
	pthread_mutex_lock(&(cdc->list_locks));
		cdc->used_data_count -= remove_from_list(&(cdc->used_list), free_data);
		cdc->free_data_count += insert_head(&(cdc->free_list), free_data);
	pthread_mutex_unlock(&(cdc->list_locks));
}

void bump_used_data_on_reuse(cashed_data_class* cdc, data* free_data)
{
	pthread_mutex_lock(&(cdc->list_locks));
		remove_from_list(&(cdc->used_list), free_data);
		insert_head(&(cdc->used_list), free_data);
	pthread_mutex_unlock(&(cdc->list_locks));
}

void release_all_free_data(cashed_data_class* cdc)
{
	pthread_mutex_lock(&(cdc->list_locks));
		while(get_head(&(cdc->free_list)) != NULL)
		{
			data* free_data = (data*) get_head(&(cdc->free_list));
			free(free_data);
			cdc->free_data_count -= remove_head(&(cdc->free_list));
		}
	pthread_mutex_unlock(&(cdc->list_locks));
}

void deinit_cashed_data_class(cashed_data_class* cdc)
{
	release_all_free_data(cdc);
	pthread_mutex_destroy(&(cdc->list_locks));
}