#ifndef CASHED_HASHTABLE_H
#define CASHED_HASHTABLE_H

#include<cutlery/cachemap.h>
#include<cutlery/pheap.h>

typedef struct cashed_hashtable cashed_hashtable;
struct cashed_hashtable
{
	// LRU cache of all the elements keyed using their keys
	cachemap hashtable;

	// min heap of the elements, with expiry_in_microseconds set
	// keyed using their expiry_in_microseconds, to evict the expired elements
	pheap expiryheap;
};

#endif