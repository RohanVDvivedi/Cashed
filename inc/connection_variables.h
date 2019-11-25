#ifndef CONNECTION_VARIABLES_H
#define CONNECTION_VARIABLES_H

#include<global_hash.h>

hashmap* get_connection_variables_hashmap();

void insert_connection_variable(dstring* variable_name, int is_shared, Data* value);

Data* get_connection_variable(dstring* variable_name);

int delete_connection_variable(dstring* variable_name);

void delete_connection_variables_hashmap(hashmap* connection_variables);

#endif