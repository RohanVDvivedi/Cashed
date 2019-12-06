#ifndef QUERY_H
#define QUERY_H

#include<dstring.h>

#include<global_hash.h>
#include<connection_variables.h>
#include<command.h>
#include<data.h>

typedef struct parameter parameter;
struct parameter
{
	int is_query;

	// this is either a query* (-> i.e. an additional sub query to solve )
	// or a dstring* (-> i.e. the name of the variable to access, from the connection_variables)
	void* value;
};

typedef struct query query;
struct query
{
	command command;

	// each of the parameter can be a variable name or a query 
	array* parameters;
};

// returns NULL, if there is parsing error for a wrong syntax query
// else will return a query
query* parse_query(dstring* requestSequence);

// this function is to delete the memory occupied by the query
void delete_query(query* query_p);

// returns 1 if client demands closing connection
int process_query(dstring* responseSequence, query* query_p, hashmap* connection_variables);

#endif