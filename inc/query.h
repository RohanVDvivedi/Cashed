#ifndef QUERY_H
#define QUERY_H

#include<dstring.h>
#include<array.h>

#include<command.h>

typedef struct query query;
struct query
{
	command cmd;
	unsigned int params_count;
	array params;
};

#include<process_query.h>

void init_query(query* query_p);

void serialize_query(dstring* str, query* query_p);

void deserialize_query(dstring* str, query* query_p);

void print_query(query* query_p);

void deinit_query(query* query_p);

#endif