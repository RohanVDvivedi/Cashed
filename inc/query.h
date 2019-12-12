#ifndef QUERY_H
#define QUERY_H

#include<dstring.h>

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

	// the number of parameters passed to the given array
	unsigned long long int parameter_count;

	// each of the parameter can be a variable name or a query 
	array* parameters;
};

// returns NULL, if there is parsing error for a wrong syntax query
// else will return a query
query* parse_query(dstring* requestSequence);

// creates a new structure to hold a query
query* get_query(dstring* command_or_datatype_name);

// creates a new structure to hold a parameter
parameter* get_parameter(parameter_type type, void* value);

// this function is to delete the memory occupied by the query
void delete_query(query* query_p);

// this function is to delete the memory occupied by the parameter, whether LITERAL or a QUERY
void delete_parameter(parameter* parameter_p);

// the insert_parameter_for_query, inserts parameter parameter_p to the list of parameters for the query query_p
void insert_parameter_for_query(query* query_p, parameter* parameter_p);

// returns 1 if client demands closing connection
// 0 for success and returns <1 for error in processing the query
// the result is the address at which the address to the data processed by 
// the query will be stored for the calling function to use
int process_query(query* query_p, hashmap* connection_variables, Data** result);

#endif