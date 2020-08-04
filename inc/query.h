#ifndef QUERY_H
#define QUERY_H

#include<dstring.h>

#include<command.h>
#include<data.h>

typedef struct query query;
struct query
{
	command cmd;
	dstring key;
	dstring value;
};

void parse_query(dstring* requestSequence, query* query_p);

void process_query(dstring* responseSequence, query* query_p);

#endif