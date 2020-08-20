#ifndef CASHED_CLIENT_H
#define CASHED_CLIENT_H

#include<transaction_client.h>
#include<cashed_result.h>
#include<cashed_query.h>

// it will only tcp/ipv4, that is what we use
transaction_client* get_cashed_client(char* host, int port, int connection_count);

// below is the declatarion of the function that is internally used, 
// for executing your query on a single connection with file discriptor fd
// if you have built a connection using the make_connection functions of simple blocking client (from ConnMan)
// or using socket library
int execute_cashed_query(int fd, query* query_p, result* result_p);

// this is the function you will pass to the transation client of the cashed, as a transaction_function of the query
void* transact_cashed_query(int fd, int* close_connection_requested, void* query_p);

// call this function to close all connections of the transaction client
void close_cashed_client(transaction_client* cashed_client);

#endif