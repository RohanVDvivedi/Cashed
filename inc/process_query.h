#ifndef PROCESS_QUERY_H
#define PROCESS_QUERY_H

#include<query.h>

// returns 1 if client demands closing connection
// 0 for success and returns <1 for error in processing the query
// the result is the address at which the address to the data processed by 
// the query will be stored for the calling function to use
int process_query(query* query_p, hashmap* connection_variables, Data** result);

#endif