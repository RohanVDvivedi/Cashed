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
	int exit_called = 0;
	switch(query_p->command)
	{
		case GET :
		{
			printf("GET\nKEY : ");
			display_dstring(query_p->key);printf("\n\n");
			break;
		}
		case SET :
		{
			printf("SET\nKEY : ");
			display_dstring(query_p->key);printf("\nVAL : ");
			display_dstring(query_p->key);printf("\n\n");
			break;
		}
		case DEL :
		{
			printf("DEL\nKEY : ");
			display_dstring(query_p->key);printf("\n\n");
			break;
		}
		case EXIT :
		default :
		{
			printf("EXIT\n");
			exit_called = 1;
			break;
		}
	}

	// delete query dstrings
	if(query_p->key != NULL)
	{
		delete_dstring(query_p->key);
	}
	if(query_p->value != NULL)
	{
		delete_dstring(query_p->value);
	}

	return exit_called;
}