#ifndef QUERY_H
#define QUERY_H

#include<main.h>
#include<command.h>
#include<dstring.h>
#include<data.h>

typedef struct query query;
struct query
{
	command command;

	Data* key;

	Data* value;
};

// this is a statefull parser for the request sequence
// it returns 1, if the request Sequence is processed enough to pass it to process, else 0
int parse_statefull_request(dstring* requestSequence, query* query_p);

// returns 1 if client demands closing connection
int process_query(dstring* responseSequence, query* query_p);

#endif