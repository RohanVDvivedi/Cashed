#include<query.h>

query* parse_query(dstring* requestSequence)
{
	// todo
	return NULL;
}

// returns NULL, for error or if the parameter is not found 
Data* get_parameter(query* query_p, unsigned long long int parameter_index, hashmap* connection_variables)
{
	parameter* parameter_p = (parameter*) get_element(query_p->parameters, parameter_index);

	Data* result = NULL;

	if(parameter_p != NULL)
	{
		// if the parameter is not a query, it is a connection variable
		if(parameter_p->type == LITERAL)
		{
			result = get_connection_variable(connection_variables, ((dstring*)(parameter_p->value)));
		}
		// if the parameter is a query, we need to solve the query to get the parameter
		else if(parameter_p->type == QUERY)
		{
			TypeOfData type = get_type_from_dstring(query_p->command_or_datatype_name);
			if(type == UNIDENTIFIED)
			{
				process_query(((query*)(parameter_p->value)), connection_variables, &result);
			}
			else
			{
				result = get_new_data(type, ((dstring*)(parameter_p->value)));
			}
		}
	}

	return result;
}

int process_query(query* query_p, hashmap* connection_variables, Data** result)
{
	int exit_called = 0;
	int error_in_processing = 0;

	switch(identify_command(query_p->command_or_datatype_name))
	{
		case GET :
		{
			Data* data_structure = get_parameter(query_p, 0, connection_variables);
			Data* key = get_parameter(query_p, 1, connection_variables);
			Data* value = GET_command(data_structure, key);
			delete_data(key);
			(*result) = value;
			break;
		}
		case SET :
		{
			Data* data_structure = get_parameter(query_p, 0, connection_variables);
			Data* key = get_parameter(query_p, 1, connection_variables);
			Data* value = get_parameter(query_p, 2, connection_variables);
			int update_was_done = SET_command(data_structure, key, value);
			if(update_was_done == 1)
			{
				delete_data(value);
				delete_data(key);
			}
			break;
		}
		case DEL :
		{
			Data* data_structure = get_parameter(query_p, 0, connection_variables);
			Data* key = get_parameter(query_p, 1, connection_variables);
			Data* return_key;
			Data* return_value;
			int elements_deleted = DEL_command(data_structure, key, &return_key, &return_value);
			if(elements_deleted == 1)
			{
				delete_data(return_key);
				delete_data(return_value);
			}
			else
			{
				error_in_processing = -1;
			}
			delete_data(key);
			break;
		}
		case EXIT :
		default :
		{
			exit_called = 1;
			break;
		}
	}

	return exit_called + error_in_processing;
}

void delete_query(query* query_p);

void delete_parameter(parameter* parameter_p)
{
	if(parameter_p->type == QUERY)
	{
		delete_query((query*)(parameter_p->value));
	}
	else if(parameter_p->type == LITERAL)
	{
		delete_dstring((dstring*)(parameter_p->value));
	}
	free(parameter_p);
}

void delete_parameter_wrapper(parameter* parameter_p, unsigned long long int index, const void* ap)
{
	if(parameter_p != NULL)
	{
		delete_parameter(parameter_p);			
	}
}

void delete_query(query* query_p)
{
	// delete all the parameters for the array
	for_each_in_array(query_p->parameters, (void (*)(void*, unsigned long long int, const void*))(delete_parameter_wrapper), NULL);
	delete_array(query_p->parameters);
	free(query_p);
}