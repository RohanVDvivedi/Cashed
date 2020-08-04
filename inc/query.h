#ifndef QUERY_H
#define QUERY_H

#include<dstring.h>

#include<command.h>
#include<data.h>

#include<result.h>

typedef struct query query;
struct query
{
	command cmd;
	dstring key;
	dstring value;
};

void init_query(query* query_p);

void serialize_query(dstring* str, query* query_p);

void deserialize_query(dstring* str, query* query_p);

void deinit_query(query* query_p);

#endif