#include<cashed_expiry_manager.h>

#include<cashed_hashtable_def.h>

static void* expiry_manager_job_function(void* cashtable_v_p)
{
	cashtable* cashtable_p = cashtable_v_p;
	c_expiry_manager* cem = &(cashtable_p->expiry_manager);

	// run the job in while 1 loop, until someone calls exit
	while(1)
	{
		pthread_mutex_lock(&(cem->expiry_heap_lock));

		// check the expiry time of the current top element
		c_data* heap_top = (c_data*) get_top_heap(&(cem->expiry_heap));
		if(heap_top != NULL && has_expiry_elapsed(heap_top))
		{
			pop_heap(&(cem->expiry_heap));
			remove_data_cashtable(cashtable_p, heap_top);
		}

		// go to sleep until that time is about to occur

		pthread_mutex_unlock(&(cem->expiry_heap_lock));
	}

	return NULL;
}

static void expiry_heap_index_update_callback_function(const void* data_v_p, unsigned int index, const void* callback_params)
{
	c_data* data_p = (c_data*) data_v_p;
	data_p->expiry_heap_manager_index = index;
}

void init_expiry_heap(c_expiry_manager* cem, unsigned int min_element_count, cashtable* cashtable_p)
{
	pthread_mutex_init(&(cem->expiry_heap_lock), NULL);
	initialize_heap(&(cem->expiry_heap), min_element_count, MIN_HEAP, (int (*)(const void*, const void*))compare_expiry, expiry_heap_index_update_callback_function, NULL);
	initialize_job(&(cem->expiry_manager_job), expiry_manager_job_function, cashtable_p);
	pthread_cond_init(&(cem->conditional_wakeup_on_expiry), NULL);

	// start executing the expiry manager job on a separate thread
	// execute_async(&(cem->expiry_manager_job));
}

void register_data_for_expiry(c_expiry_manager* cem, c_data* data_p)
{
	// can not register a data with no expiry
	if(data_p->expiry_seconds == -1)
		return;

	pthread_mutex_lock(&(cem->expiry_heap_lock));

	// wake up the expiry manager thread only if, you may be inserting to the top of the expiry heap
	int expiry_manager_job_wakeup_required = 0;
	if(get_top_heap(&(cem->expiry_heap)) == NULL || compare_expiry(get_top_heap(&(cem->expiry_heap)), data_p) > 0)
		expiry_manager_job_wakeup_required = 1;

	// insert the data element that needs to be expired in future
	push_heap(&(cem->expiry_heap), data_p);

	// wake up the sleeping job thread to check for the new data
	if(expiry_manager_job_wakeup_required == 1)
		pthread_cond_signal(&(cem->conditional_wakeup_on_expiry));

	pthread_mutex_unlock(&(cem->expiry_heap_lock));
}

void de_register_data_from_expiry_heap(c_expiry_manager* cem, c_data* data_p)
{
	// can not deregister a data with no expiry
	if(data_p->expiry_seconds == -1)
		return;

	pthread_mutex_lock(&(cem->expiry_heap_lock));

	// exit, if the top of the heap is NULL (i.e. heap is empty)
	// or if the data_p does not exist in the heap
	// i.e. exist at the same index as it is mentioned on its heap_index
	if( get_top_heap(&(cem->expiry_heap)) == NULL ||
		get_element(&(cem->expiry_heap_lock.holder), data_p->expiry_heap_manager_index) != data_p)
	{
		pthread_mutex_unlock(&(cem->expiry_heap_lock));
		return;
	}

	// wake up the expiry manager thread only if, you are removing the top element of the expiry heap
	int expiry_manager_job_wakeup_required = 0;
	if(get_top_heap(&(cem->expiry_heap)) == data_p)
		expiry_manager_job_wakeup_required = 1;

	// call remove from heap function
	remove_from_heap(&(cem->expiry_heap), data_p->expiry_heap_manager_index);

	// wake up the sleeping job thread to check for the new data, and may be sleep again, knowing what came in
	if(expiry_manager_job_wakeup_required == 1)
		pthread_cond_signal(&(cem->conditional_wakeup_on_expiry));

	pthread_mutex_unlock(&(cem->expiry_heap_lock));
}

void deinit_expiry_heap(c_expiry_manager* cem)
{
	// ask the expiry_manager_job_function to die
	// wait for it to die

	deinitialize_heap(&(cem->expiry_heap));
	pthread_mutex_destroy(&(cem->expiry_heap_lock));
	pthread_cond_destroy(&(cem->conditional_wakeup_on_expiry));
}