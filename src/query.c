#include<query.h>

int parse_statefull_request(dstring* requestSequence, query* query_p)
{
	if(requestSequence->cstring[1] == 'E')
	{
		if(requestSequence->cstring[2] == 'T')
		{
			if(requestSequence->cstring[0] == 'G')
			{
				query_p->command = GET;
			}
			else if(requestSequence->cstring[0] == 'S')
			{
				query_p->command = SET;
			}
		}
		else if(requestSequence->cstring[2] == 'L')
		{
			query_p->command = DEL;
		}
	}
	else if(requestSequence->cstring[1] == 'X')
	{
		query_p->command = EXIT;
	}

	query_p->key = NULL;
	query_p->value = NULL;

	if(query_p->command != EXIT)
	{
		query_p->key = get_dstring("", 10);

		char char_str[2] = "x";
		unsigned long long int iter = 4;

		while(requestSequence->cstring[iter] != '\0' && requestSequence->cstring[iter] != '\n' && requestSequence->cstring[iter] != '\r' && requestSequence->cstring[iter] == ' '){iter++;}
		while(requestSequence->cstring[iter] != '\0' && requestSequence->cstring[iter] != '\n' && requestSequence->cstring[iter] != '\r' && requestSequence->cstring[iter] != ' ')
		{
			char_str[0] = requestSequence->cstring[iter];
			query_p->key = append_to_dstring(query_p->key, char_str);
			iter++;
		}

		if(query_p->command == SET)
		{
			query_p->value = get_dstring("", 10);
			while(requestSequence->cstring[iter] != '\0' && requestSequence->cstring[iter] != '\n' && requestSequence->cstring[iter] != '\r' && requestSequence->cstring[iter] == ' '){iter++;}
			while(requestSequence->cstring[iter] != '\0' && requestSequence->cstring[iter] != '\n' && requestSequence->cstring[iter] != '\r' && requestSequence->cstring[iter] != ' ')
			{
				char_str[0] = requestSequence->cstring[iter];
				query_p->value = append_to_dstring(query_p->value, char_str);
				iter++;
			}
		}
	}

	return 1;
}

int process_query(dstring* responseSequence, query* query_p)
{
	make_dstring_empty(responseSequence);

	int exit_called = 0;

	int delete_query_key = 0;
	int delete_query_value = 0;


	switch(query_p->command)
	{
		case GET :
		{
			Data* key = get_new_data(query_p->key);
			Data* value = (Data*)(find_value_from_hash(hashTable, query_p->key));
			delete_data(key);
			serialize_data(responseSequence, value);
			break;
		}
		case SET :
		{
			Data* key = get_new_data(query_p->key);
			Data* value = (Data*)(find_value_from_hash(hashTable, query_p->key));
			if(value == NULL)
			{
				value = get_new_data(query_p->value);
				insert_entry_in_hash(hashTable, key, value);
				responseSequence = append_to_dstring(responseSequence, "INSERTED");
			}
			else
			{
				Data* value_new = get_new_data(query_p->value);
				transfer_data(value, value_new);
				delete_data(value_new);
				responseSequence = append_to_dstring(responseSequence, "UPDATED");
			}
			delete_data(key);
			break;
		}
		case DEL :
		{
			Data* key = get_new_data(query_p->key);
			Data* return_key;
			Data* return_value;
			delete_entry_from_hash(hashTable, query_p->key, (const void**)(&return_key), (const void**)(&return_value));
			delete_data(return_key);
			delete_data(return_value);
			delete_data(key);
			responseSequence = append_to_dstring(responseSequence, "DELETED");
			break;
		}
		case EXIT :
		default :
		{
			exit_called = 1;
			break;
		}
	}

	responseSequence = append_to_dstring(responseSequence, "\r\n");

	if(query_p->key != NULL)
	{
		delete_dstring(query_p->key);
		query_p->key = NULL;
	}
	if(query_p->value != NULL)
	{
		delete_dstring(query_p->value);
		query_p->value = NULL;
	}

	return exit_called;
}