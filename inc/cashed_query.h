#ifndef QUERY_H
#define QUERY_H

#include<dstring.h>
#include<array.h>

#include<cashed_command.h>

typedef struct c_query c_query;
struct c_query
{
	c_command cmd;
	unsigned int params_count;
	dstring* params;

	// this is the maximum number of parameters that can be accomodated in params
	unsigned int params_capacity;
};

void init_query(c_query* query_p, c_command cmd);

void add_query_param(c_query* query_p, dstring* new_param);

const dstring* get_query_param(const c_query* query_p, unsigned int index);

void serialize_query(dstring* str, c_query* query_p);

void deserialize_query(dstring* str, c_query* query_p);

void print_query(c_query* query_p);

void deinit_query(c_query* query_p);

#endif