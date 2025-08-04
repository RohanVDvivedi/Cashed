#include<cashed/cashed_hashtable.h>

#include<cashed/cashed_data.h>
#include<cashed/cashed_expiry_manager.h>

#include<posixutils/pthread_cond_utils.h>

#include<stdlib.h>

/*
	INTERNAL FUNCTIONS - START
*/

cashed_data* get_from_cashed_hashtable_INTERNAL(cashed_hashtable* chtbl, const cashed_data* data)
{
	cashed_data* result = (cashed_data*) find_equals_in_cachemap(&(chtbl->lruhashtable), data);
	bump_element_in_cachemap(&(chtbl->lruhashtable), result);
	return result;
}

int insert_in_cashed_hashtable_INTERNAL(cashed_hashtable* chtbl, cashed_data* data)
{
	if(!insert_in_cachemap(&(chtbl->lruhashtable), data))
		return 0;
	if(data->absolute_expiry_in_microseconds != BLOCKING)
	{
		push_to_pheap(&(chtbl->expiryheap), data);
		if(data == get_top_of_pheap(&(chtbl->expiryheap))) // if the inserted element is the top element for expiry, then wake up alarm job
			wake_up_alarm_job(chtbl->expiry_manager);
	}
	return 1;
}

int remove_from_cashed_hashtable_INTERNAL(cashed_hashtable* chtbl, cashed_data* data)
{
	if(!remove_from_cachemap(&(chtbl->lruhashtable), data))
		return 0;
	int is_data_top_of_expiry_pheap = (data == get_top_of_pheap(&(chtbl->expiryheap))); // if the element was the top element for expiry, then wake up alarm job
	if(remove_from_pheap(&(chtbl->expiryheap), data) && is_data_top_of_expiry_pheap)
		wake_up_alarm_job(chtbl->expiry_manager);
	return 1;
}

/*
	INTERNAL FUNCTIONS - END
*/

void initialize_cashed_hashtable(cashed_hashtable* chtbl, cy_uint bucket_count)
{
	if(!initialize_cachemap(&(chtbl->lruhashtable), NULL, NEVER_PINNED, bucket_count, &simple_hasher(hash_cashed_data_using_key), &simple_comparator(compare_cashed_data_using_key), offsetof(cashed_data, embed_node1)))
		exit(-1);

	initialize_pheap(&(chtbl->expiryheap), MIN_HEAP, LEFTIST, &simple_comparator(compare_cashed_data_using_expiry), offsetof(cashed_data, embed_node2));

	chtbl->expiry_manager = new_alarm_job(expiry_manager_alarm_job_function, chtbl);
	if(chtbl->expiry_manager == NULL)
		exit(-1);
}