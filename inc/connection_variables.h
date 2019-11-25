#ifndef CONNECTION_VARIABLES_H
#define CONNECTION_VARIABLES_H

#include<stdlib.h>

#include<jenkinshash.h>
#include<global_hash.h>

hashmap* get_connection_variables_hashmap();

void set_connection_variable(hashmap* connection_variables, dstring* var_name, int is_shared, Data* value);

Data* get_connection_variable(hashmap* connection_variables, dstring* var_nam);

int delete_connection_variable(hashmap* connection_variables, dstring* var_nam);

void delete_connection_variables_hashmap(hashmap* connection_variables);

#endif