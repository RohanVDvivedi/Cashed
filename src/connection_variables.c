#include<connection_variables.h>

// we are not storing the variable name directly as a dstring, but a structure composed of is_shared int
// is_shared int states if the connection variable would exists in the scope of other connections
// if this is true i.e. 1, we are not suppossed to delete the Data* that exists corresponding to the variable name
typedef struct variable_name variable_name;
struct variable_name
{
	dstring* name;
	int is_shared;
};

variable_name* get_variable_name(char* name, int is_shared)
{
	variable_name* var_nam = malloc(sizeof(variable_name));
	var_nam->name = get_dstring(name, 10);
	var_nam->is_shared = is_shared;
	return var_nam;
}

void delete_variable_name(variable_name* var_nam)
{
	delete_dstring(var_nam->name);
	free(var_nam);
}
/* variable name functions complete */

/* utility functions used by the connection variable hash -> */
unsigned long long int variable_name_hash(variable_name* var_nam)
{
	return jenkins_hash(var_nam->name->cstring, var_nam->name->bytes_occupied);
}

int variable_name_compare(variable_name* var_nam1, variable_name* var_nam2)
{
	return compare_dstring(var_nam1->name, var_nam2->name);
}
/* <- utility functions used by the connection variable hash */

hashmap* get_connection_variables_hashmap()
{
	hashmap* connection_variables = get_hashmap(10, (unsigned long long int (*)(const void*))variable_name_hash, (int (*)(const void*, const void*))variable_name_compare, ELEMENTS_AS_RED_BLACK_BST);
	insert_entry_in_hash(connection_variables, get_variable_name("global_hash", 1), global_hash);
	return connection_variables;
}

void set_connection_variable(hashmap* connection_variables, dstring* var_nam, int is_shared, Data* value)
{
	variable_name* var_nam_key = get_variable_name(var_nam->cstring, is_shared);
	if(find_value_from_hash(connection_variables, var_nam_key) == NULL)
	{
		insert_entry_in_hash(connection_variables, var_nam_key, value);
	}
	else
	{
		update_value_in_hash(connection_variables, var_nam_key, value, NULL);
		delete_variable_name(var_nam_key);
	}
}

Data* get_connection_variable(hashmap* connection_variables, dstring* var_nam)
{
	variable_name var_nam_key = {.name = var_nam, .is_shared = 1};
	Data* result = (Data*)find_value_from_hash(connection_variables, &var_nam_key);
	return result;
}

int delete_connection_variable(hashmap* connection_variables, dstring* var_nam)
{
	variable_name var_nam_key = {.name = var_nam, .is_shared = 1};
	variable_name* return_key = NULL;
	Data* return_value = NULL;
	int result = delete_entry_from_hash(connection_variables, &var_nam_key, (const void**)(&return_key), (const void**)(&return_value));
	if(result > 0)
	{
		if(return_key->is_shared == 0)
		{
			delete_data(return_value);
		}
		delete_variable_name(return_key);
	}
	return result;
}

void delete_connection_variable_wrapper(const void* key, const void* value, const void* additional_params)
{
	if(((variable_name*)key)->is_shared == 0)
	{
		delete_data(((Data*)value));
	}
	delete_variable_name((variable_name*)key);
}

void delete_connection_variables_hashmap(hashmap* connection_variables)
{
	for_each_entry_in_hash(connection_variables, delete_connection_variable_wrapper, NULL);
}