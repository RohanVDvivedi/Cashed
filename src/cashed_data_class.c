#include<cashed_data_class.h>

#include<stdlib.h>
#include<stddef.h>

void init_data_class(c_data_class* cdc, unsigned int total_data_size)
{
	cdc->total_data_size = total_data_size;
	pthread_mutex_init(&(cdc->list_locks), NULL);
	cdc->used_data_count = 0;
	initialize_linkedlist(&(cdc->used_list), offsetof(c_data, data_class_llnode));
	cdc->free_data_count = 0;
	initialize_linkedlist(&(cdc->free_list), offsetof(c_data, data_class_llnode));
	initialize_bstnode(&(cdc->data_manager_bstnode));
}

c_data* get_cached_data(c_data_class* cdc)
{
	pthread_mutex_lock(&(cdc->list_locks));
	c_data* new_data = NULL;
	if(cdc->free_data_count > 0)
	{
		new_data = (c_data*) get_head_of_linkedlist(&(cdc->free_list));
		cdc->free_data_count -= remove_head_from_linkedlist(&(cdc->free_list));
	}
	else
	{
		new_data = malloc(cdc->total_data_size);
		init_data(new_data, cdc);
	}
	if(new_data != NULL)
		cdc->used_data_count += insert_head_in_linkedlist(&(cdc->used_list), new_data);
	pthread_mutex_unlock(&(cdc->list_locks));
	return new_data;
}

void return_used_data(c_data_class* cdc, c_data* free_data)
{
	pthread_mutex_lock(&(cdc->list_locks));
	cdc->used_data_count -= remove_from_linkedlist(&(cdc->used_list), free_data);
	cdc->free_data_count += insert_head_in_linkedlist(&(cdc->free_list), free_data);
	pthread_mutex_unlock(&(cdc->list_locks));
}

void bump_used_data_on_reuse(c_data_class* cdc, c_data* used_data)
{
	pthread_mutex_lock(&(cdc->list_locks));
	remove_from_linkedlist(&(cdc->used_list), used_data);
	insert_head_in_linkedlist(&(cdc->used_list), used_data);
	pthread_mutex_unlock(&(cdc->list_locks));
}

void release_all_free_data(c_data_class* cdc)
{
	pthread_mutex_lock(&(cdc->list_locks));
	while(get_head_of_linkedlist(&(cdc->free_list)) != NULL)
	{
		c_data* free_data = (c_data*) get_head_of_linkedlist(&(cdc->free_list));
		cdc->free_data_count -= remove_head_from_linkedlist(&(cdc->free_list));
		free(free_data);
	}
	pthread_mutex_unlock(&(cdc->list_locks));
}

void deinit_data_class(c_data_class* cdc)
{
	release_all_free_data(cdc);
	pthread_mutex_destroy(&(cdc->list_locks));
}

int compare_data_classes_on_total_data_size(const c_data_class* cdc1, const c_data_class* cdc2)
{
	if(cdc1->total_data_size > cdc2->total_data_size)
		return 1;
	else if(cdc1->total_data_size < cdc2->total_data_size)
		return -1;
	return 0;
}