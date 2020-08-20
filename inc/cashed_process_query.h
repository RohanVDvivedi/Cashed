#ifndef PROCESS_QUERY_H
#define PROCESS_QUERY_H

#include<query.h>
#include<result.h>

#include<cashed_hashtable.h>

void process_query(cashtable* cashtable_p, query* query_p, result* result_p);

#endif