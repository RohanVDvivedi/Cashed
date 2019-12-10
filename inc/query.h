#ifndef QUERY_H
#define QUERY_H

#include<dstring.h>

#include<global_hash.h>
#include<connection_variables.h>
#include<command.h>
#include<data.h>

typedef enum parameter_type parameter_type;
enum parameter_type
{
	QUERY,
	LITERAL // literals are stored as dstring
};

typedef struct parameter parameter;
struct parameter
{
	parameter_type type;

	// this is either a query* (-> i.e. an additional sub query to solve )
	// or a dstring* (-> i.e. the name of the variable to access, from the connection_variables)
	void* value;
};

typedef struct query query;
struct query
{
	dstring* command_or_datatype_name;

	// each of the parameter can be a variable name or a query 
	array* parameters;
};

// returns NULL, if there is parsing error for a wrong syntax query
// else will return a query
query* parse_query(dstring* requestSequence);

// this function is to delete the memory occupied by the query
void delete_query(query* query_p);

// returns 1 if client demands closing connection
// 0 for success and returns <1 for error in processing the query
// the result is the address at which the address to the data processed by 
// the query will be stored for the calling function to use
int process_query(query* query_p, hashmap* connection_variables, Data** result);

#endif