#include<cashed_data_class.h>

#include<stdlib.h>
#include<stddef.h>

void init_data_class(c_data_class* cdc, unsigned int total_data_size, unsigned int slab_size, unsigned int max_memory_usage)
{
	cdc->total_data_size = total_data_size;
	pthread_mutex_init(&(cdc->lru_lock), NULL);
	initialize_linkedlist(&(cdc->lru), offsetof(c_data, data_class_llnode));
	cache_create(&(cdc->slab_allocator_cache), slab_size, total_data_size, max_memory_usage, NULL, NULL);
	initialize_bstnode(&(cdc->data_manager_bstnode));
}

c_data* allocate_data(c_data_class* cdc, int* needs_eviction)
{
	c_data* new_data = cache_alloc(&(cdc->slab_allocator_cache));

	pthread_mutex_lock(&(cdc->lru_lock));
	if(new_data != NULL)	// initialize data and insert it to lru
	{
		*needs_eviction = 0;
		init_data(new_data, cdc);
		insert_head_in_linkedlist(&(cdc->lru), new_data);
	}
	else	// return a data of the same size that is a good candidate for eviction
	{
		*needs_eviction = 1;
		new_data = (c_data*) get_tail_of_linkedlist(&(cdc->lru));
		remove_tail_from_linkedlist(&(cdc->lru));
	}
	pthread_mutex_unlock(&(cdc->lru_lock));

	return new_data;
}

void deallocate_data(c_data_class* cdc, c_data* free_data)
{
	pthread_mutex_lock(&(cdc->lru_lock));
	remove_from_linkedlist(&(cdc->lru), free_data);
	pthread_mutex_unlock(&(cdc->lru_lock));

	cache_free(&(cdc->slab_allocator_cache), free_data);
}

void bump_used_data_on_reuse(c_data_class* cdc, c_data* used_data)
{
	pthread_mutex_lock(&(cdc->lru_lock));
	remove_from_linkedlist(&(cdc->lru), used_data);
	insert_head_in_linkedlist(&(cdc->lru), used_data);
	pthread_mutex_unlock(&(cdc->lru_lock));
}

void deinit_data_class(c_data_class* cdc)
{
	// free all data in LRU
	pthread_mutex_lock(&(cdc->lru_lock));
	while(get_head_of_linkedlist(&(cdc->lru)) != NULL)
	{
		c_data* free_data = (c_data*) get_head_of_linkedlist(&(cdc->lru));
		remove_head_from_linkedlist(&(cdc->lru));
		cache_free(&(cdc->slab_allocator_cache), free_data);
	}
	pthread_mutex_unlock(&(cdc->lru_lock));

	// deinitialize slab_allocator_cache
	cache_destroy(&(cdc->slab_allocator_cache));

	pthread_mutex_destroy(&(cdc->lru_lock));
}

int compare_data_classes_on_total_data_size(const c_data_class* cdc1, const c_data_class* cdc2)
{
	if(cdc1->total_data_size > cdc2->total_data_size)
		return 1;
	else if(cdc1->total_data_size < cdc2->total_data_size)
		return -1;
	return 0;
}