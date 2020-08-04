#ifndef CASHED_CLIENT_H
#define CASHED_CLIENT_H

#include<transaction_client.h>
#include<result.h>
#include<query.h>

// only tcp/ipv4
transaction_client* get_cashed_client(char* host, int port, int connection_count);

int execute_query(int fd, query* query_p, result* result_p);

result* transact_query(int fd, int* close_connection_requested, query* query_p);

void close_cashed_client(transaction_client* cashed_client);

#endif