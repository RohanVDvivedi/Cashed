#include<cashed_expiry_manager.h>

#include<cashed_hashtable_def.h>

#include<stddef.h>

static void* expiry_manager_job_function(void* cashtable_v_p)
{
	cashtable* cashtable_p = cashtable_v_p;
	c_expiry_manager* cem = &(cashtable_p->expiry_manager);

	// run the job loop, until someone calls exit
	while(!(cem->expiry_manager_job_shutdown_called))
	{
		// reading the top from heap
		pthread_mutex_lock(&(cem->expiry_heap_lock));
		c_data* heap_top = (c_data*) get_top_of_heap(&(cem->expiry_heap));
		if(heap_top == NULL)
		{
			pthread_cond_wait(&(cem->conditional_wakeup_on_expiry), &(cem->expiry_heap_lock));
		}
		else if(!has_expiry_elapsed(heap_top))
		{
			struct timespec wake_up_at = heap_top->set_up_time;
			wake_up_at.tv_sec += heap_top->expiry_seconds;
			pthread_cond_timedwait(&(cem->conditional_wakeup_on_expiry), &(cem->expiry_heap_lock), &wake_up_at);
		}
		pthread_mutex_unlock(&(cem->expiry_heap_lock));

		// we can not remove a NULL data
		if(heap_top == NULL)
			continue;

		// fund appropriate bucket responsible for holding the data
		unsigned int index = hash_data(heap_top) % cashtable_p->bucket_count;
		c_bucket* bucket = cashtable_p->buckets + index;

		// removal from hashtable
		write_lock(&(bucket->data_list_lock));

		pthread_mutex_lock(&(cem->expiry_heap_lock));
		if(heap_top == get_top_of_heap(&(cem->expiry_heap)) && has_expiry_elapsed(heap_top))
		{
			pop_from_heap(&(cem->expiry_heap));
			if(get_capacity_heap(&(cem->expiry_heap)) > 3 * get_element_count_heap(&(cem->expiry_heap)))
				shrink_heap(&(cem->expiry_heap));
			pthread_mutex_unlock(&(cem->expiry_heap_lock));

			remove_bucket_data_unsafe(bucket, heap_top);

			return_used_data(heap_top->data_class, heap_top);
		}
		else
			pthread_mutex_unlock(&(cem->expiry_heap_lock));

		write_unlock(&(bucket->data_list_lock));
	}

	return NULL;
}

void init_expiry_heap(c_expiry_manager* cem, unsigned int min_element_count, cashtable* cashtable_p)
{
	pthread_mutex_init(&(cem->expiry_heap_lock), NULL);
	initialize_heap(&(cem->expiry_heap), min_element_count, MIN_HEAP, (int (*)(const void*, const void*))compare_expiry, offsetof(c_data, expiry_manager_hpnode));
	initialize_promise(&(cem->expiry_manager_job_completion_promise));
	initialize_job(&(cem->expiry_manager_job), expiry_manager_job_function, cashtable_p, &(cem->expiry_manager_job_completion_promise));
	pthread_cond_init(&(cem->conditional_wakeup_on_expiry), NULL);

	// start executing the expiry manager job on a separate thread
	cem->expiry_manager_job_shutdown_called = 0;
	execute_async(&(cem->expiry_manager_job));
}

void register_data_for_expiry(c_expiry_manager* cem, c_data* data_p)
{
	pthread_mutex_lock(&(cem->expiry_heap_lock));

	// wake up the expiry manager thread only if, you may be inserting to the top of the expiry heap
	int expiry_manager_job_wakeup_required = 0;
	if(get_top_of_heap(&(cem->expiry_heap)) == NULL || compare_expiry(get_top_of_heap(&(cem->expiry_heap)), data_p) > 0)
		expiry_manager_job_wakeup_required = 1;

	// insert the data element that needs to be expired in future
	if(is_full_heap(&(cem->expiry_heap)))
		expand_heap(&(cem->expiry_heap));
	push_to_heap(&(cem->expiry_heap), data_p);

	// wake up the sleeping job thread to check for the new data
	if(expiry_manager_job_wakeup_required == 1)
		pthread_cond_signal(&(cem->conditional_wakeup_on_expiry));

	pthread_mutex_unlock(&(cem->expiry_heap_lock));
}

void de_register_data_from_expiry_heap(c_expiry_manager* cem, c_data* data_p)
{
	pthread_mutex_lock(&(cem->expiry_heap_lock));

	// call remove from heap function
	// this function is a NOP if the data_p is not in heap
	remove_from_heap(&(cem->expiry_heap), data_p);

	pthread_mutex_unlock(&(cem->expiry_heap_lock));
}

void deinit_expiry_heap(c_expiry_manager* cem)
{
	cem->expiry_manager_job_shutdown_called = 1;
	pthread_cond_signal(&(cem->conditional_wakeup_on_expiry));
	get_promised_result(&(cem->expiry_manager_job_completion_promise));

	deinitialize_job(&(cem->expiry_manager_job));

	deinitialize_heap(&(cem->expiry_heap));
	pthread_mutex_destroy(&(cem->expiry_heap_lock));
	pthread_cond_destroy(&(cem->conditional_wakeup_on_expiry));
}