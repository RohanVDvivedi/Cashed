#include<query.h>

// returns NULL, for error or if the parameter is not found 
Data* resolve_parameter(query* query_p, unsigned long long int parameter_index, hashmap* connection_variables)
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
			query* query_to_process = ((query*)(parameter_p->value));
			TypeOfData type = get_type_from_dstring(query_to_process->command_or_datatype_name);
			if(type == UNIDENTIFIED)
			{
				process_query(query_to_process, connection_variables, &result);
			}
			else
			{
				parameter* parameter_literal_expected = (parameter*) get_element(query_to_process->parameters, 0);
				if(parameter_literal_expected != NULL && parameter_literal_expected->type == LITERAL)
				{
					result = get_new_data(type, (dstring*)(parameter_literal_expected->value));
				}
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
			Data* data_structure = resolve_parameter(query_p, 0, connection_variables);
			Data* key = resolve_parameter(query_p, 1, connection_variables);
			Data* value = GET_command(data_structure, key);
			delete_data(key);
			(*result) = value;
			break;
		}
		case SET :
		{
			Data* data_structure = resolve_parameter(query_p, 0, connection_variables);
			Data* key = resolve_parameter(query_p, 1, connection_variables);
			Data* value = resolve_parameter(query_p, 2, connection_variables);
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
			Data* data_structure = resolve_parameter(query_p, 0, connection_variables);
			Data* key = resolve_parameter(query_p, 1, connection_variables);
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