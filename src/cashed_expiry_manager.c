#include<cashed_expiry_manager.h>

#include<cashed_hashtable_def.h>

static void* expiry_manager_job_function(void* cem_v_p)
{
	c_expiry_manager* cem = cem_v_p;

	// run the job in while 1 loop, until someone calls exit
	while(1)
	{
		pthread_mutex_lock(&(cem->expiry_heap_lock));

		// check the expiry time of the current top element
		c_data* heap_top = (c_data*) get_top_heap(&(cem->expiry_heap));
		if(heap_top != NULL && has_expiry_elapsed(heap_top))
		{
			pop_heap(&(cem->expiry_heap));
			//remove_data_cashtable(cashtable* cashtable_p, heap_top);
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

void init_expiry_heap(c_expiry_manager* cem, unsigned int min_element_count)
{
	pthread_mutex_init(&(cem->expiry_heap_lock), NULL);
	initialize_heap(&(cem->expiry_heap), min_element_count, MIN_HEAP, (int (*)(const void*, const void*))compare_expiry, expiry_heap_index_update_callback_function, NULL);
	initialize_job(&(cem->expiry_manager_job), expiry_manager_job_function, cem);
	pthread_cond_init(&(cem->conditional_wakeup_on_expiry), NULL);
}

void register_data_for_expiry(c_expiry_manager* cem, c_data* data_p)
{
	// do not insert any element in the expiry heap, if they do not have an expiry time
	if(data_p->expiry_seconds == -1)
		return;

	pthread_mutex_lock(&(cem->expiry_heap_lock));

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

void deinit_expiry_heap(c_expiry_manager* cem)
{
	// ask the expiry_manager_job_function to die
	// wait for it to die

	deinitialize_heap(&(cem->expiry_heap));
	pthread_mutex_destroy(&(cem->expiry_heap_lock));
	pthread_cond_destroy(&(cem->conditional_wakeup_on_expiry));
}