#include<global_hash.h>

Data* global_hash;

void init_global_hash()
{
	global_hash = malloc(sizeof(Data));
	global_hash->type = HASHTABLE;
	global_hash->rwL = get_rwlock();
	global_hash->value = get_hashmap(HASHSLOTS, (unsigned long long int (*)(const void*))hash_data, (int (*)(const void*, const void*))compare_data, ELEMENTS_AS_RED_BLACK_BST);
}