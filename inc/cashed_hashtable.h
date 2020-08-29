#ifndef CASHED_HASHTABLE_H
#define CASHED_HASHTABLE_H

#include<dstring.h>

#include<pthread.h>

/*
** cashtable is indeed a hashtable, cashtable will just help you against name collisions in your code
** also cashtable sounds cool, isn't it? LOL
*/

// hashtable is bound to provide uniqueness using the get, set and del functions

// the data is organized as a hashtable,
// each bucket is a singly linkedlist
// each bucket is protected using a lock

typedef struct cashtable cashtable;

cashtable* get_cashtable(unsigned int bucket_count);

// returns 0, if the data was not found
// the contents of the value will be appended to the return_value dstring
int get_value_cashtable(cashtable* cashtable_p, const dstring* key, dstring* return_value);

// a key value is inserted or updated by this call
// returns 1 if it was insert/updated, else it returns 0 for failure
int set_key_value_expiry_cashtable(cashtable* cashtable_p, const dstring* key, const dstring* value, int expiry_seconds);

// returns 0, if the no data was found to delete by the given key
int del_key_value_cashtable(cashtable* cashtable_p, const dstring* key);

void delete_cashtable(cashtable* cashtable_p);

#endif