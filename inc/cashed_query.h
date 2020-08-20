#ifndef QUERY_H
#define QUERY_H

#include<dstring.h>
#include<array.h>

#include<command_def.h>

typedef struct query query;
struct query
{
	command cmd;
	unsigned int params_count;
	array params;
};

void init_query(query* query_p, command cmd);

void add_query_param(query* query_p, dstring* new_param);

const dstring* get_query_param(const query* query_p, unsigned int index);

void serialize_query(dstring* str, query* query_p);

void deserialize_query(dstring* str, query* query_p);

void print_query(query* query_p);

void deinit_query(query* query_p);

#endif