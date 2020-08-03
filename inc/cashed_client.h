#ifndef CASHED_CLIENT_H
#define CASHED_CLIENT_H

#include<transaction_client.h>

// only tcp/ipv4
transaction_client* get_cashed_client(char* host, int port, int connection_count);

dstring* GET_cashed(transaction_client* cashed_client, dstring* key);

int SET_cashed(transaction_client* cashed_client, dstring* key, dstring* value);

int DEL_cashed(transaction_client* cashed_client, dstring* key);

void close_cashed_client(transaction_client* cashed_client);

#endif