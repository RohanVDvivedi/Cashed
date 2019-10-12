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
			dstring* value = (dstring*)(find_value_from_hash(hashTable, query_p->key));
			delete_query_key = 1;
			delete_query_value = 0;
			if(value != NULL)
			{
				responseSequence = append_to_dstring(responseSequence, "\"");
				responseSequence = concatenate_dstring(responseSequence, value);
				responseSequence = append_to_dstring(responseSequence, "\"");
			}
			else
			{
				responseSequence = append_to_dstring(responseSequence, "NULL");
			}
			responseSequence = append_to_dstring(responseSequence, "\n");
			break;
		}
		case SET :
		{
			dstring* value = (dstring*)(find_value_from_hash(hashTable, query_p->key));
			if(value == NULL)
			{
				insert_entry_in_hash(hashTable, query_p->key, query_p->value);
				delete_query_key = 0;
				delete_query_value = 0;
				responseSequence = append_to_dstring(responseSequence, "ENTRY INSERTED\n");
			}
			else
			{
				make_dstring_empty(value);concatenate_dstring(value, query_p->value);
				delete_query_key = 1;
				delete_query_value = 1;
				responseSequence = append_to_dstring(responseSequence, "ENTRY UPDATED\n");
			}
			break;
		}
		case DEL :
		{
			dstring* return_key;
			dstring* return_value;
			delete_entry_from_hash(hashTable, query_p->key, (const void**)(&return_key), (const void**)(&return_value));
			delete_query_key = 1;
			delete_query_value = 0;
			responseSequence = append_to_dstring(responseSequence, "ENTRY DELETED\n");
			delete_dstring(return_key);
			delete_dstring(return_value);
			break;
		}
		case EXIT :
		default :
		{
			exit_called = 1;
			break;
		}
	}

	if(delete_query_key && query_p->key != NULL)
	{
		delete_dstring(query_p->key);
		query_p->key = NULL;
	}
	if(delete_query_value && query_p->value != NULL)
	{
		delete_dstring(query_p->value);
		query_p->value = NULL;
	}

	return exit_called;
}