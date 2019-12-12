#ifndef CONNECTION_VARIABLES_H
#define CONNECTION_VARIABLES_H

#include<stdlib.h>

#include<jenkinshash.h>
#include<global_hash.h>

// for every new connection, call the get_connection_variables_hashmap function to give a new connection variable hashmap to it, as a new scope
hashmap* get_connection_variables_hashmap();

// to add a connection variable
void set_connection_variable(hashmap* connection_variables, dstring* var_name, int is_shared, Data* value);

// to retrieve a connection variable
Data* get_connection_variable(hashmap* connection_variables, dstring* var_nam);

// to delete the connection varibale hashmap
int delete_connection_variable(hashmap* connection_variables, dstring* var_nam);

// to remove all the connection variables, and delete the hasmap maintaining it
// this must be called when the connection is to be closed
void delete_connection_variables_hashmap(hashmap* connection_variables);

#endif