#ifndef GLOBAL_HASH_H
#define GLOBAL_HASH_H

#include<hashmap.h>

#include<data.h>

extern Data* global_hash;

#define HASHSLOTS 3000

void init_global_hash();

#endif