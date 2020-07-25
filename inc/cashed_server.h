#ifndef CASHED_SERVER_H
#define CASHED_SERVER_H

#include<server.h>
#include<hashmap.h>
#include<query.h>
#include<data.h>

#define HASH_POWER 10
#define HASHSLOTS (1 << HASH_POWER)

extern hashmap* hashTable;

#endif