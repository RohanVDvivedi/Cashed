#include<cashed/cashed_hashtable.h>

#include<cashed/cashed_data.h>

/*
	INTERNAL FUNCTIONS - START
*/

cashed_data* get_from_cashed_hashtable_INTERNAL(cashed_hashtable* chtbl, const cashed_data* data);

// below function does not care about duplicates
int insert_in_cashed_hashtable_INTERNAL(cashed_hashtable* chtbl, cashed_data* data);

int remove_from_cashed_hashtable_INTERNAL(cashed_hashtable* chtbl, cashed_data* data);

/*
	INTERNAL FUNCTIONS - END
*/