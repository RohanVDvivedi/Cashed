#include<cashed/cashed_hashtable.h>

#include<cashed/cashed_data.h>

#include<posixutils/pthread_cond_utils.h>

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