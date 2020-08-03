#include<query.h>

int parse_query(dstring* requestSequence, query* query_p)
{
	query_p->command = identify_command(requestSequence);

	if(query_p->command != ERR && query_p->command != ERROR)
	{
		init_dstring(&(query_p->key), "", 0);

		char char_str[2] = "x";
		unsigned long long int iter = 4;

		while(requestSequence->cstring[iter] != '\0' && requestSequence->cstring[iter] != '\n' && requestSequence->cstring[iter] != '\r' && requestSequence->cstring[iter] == ' '){iter++;}
		while(requestSequence->cstring[iter] != '\0' && requestSequence->cstring[iter] != '\n' && requestSequence->cstring[iter] != '\r' && ((iter == 0) || (iter > 0 && requestSequence->cstring[iter-1] != ')')))
		{
			char_str[0] = requestSequence->cstring[iter];
			append_to_dstring(&(query_p->key), char_str);
			iter++;
		}

		if(query_p->command == SET)
		{
			init_dstring(&(query_p->value), "", 0);
			while(requestSequence->cstring[iter] != '\0' && requestSequence->cstring[iter] != '\n' && requestSequence->cstring[iter] != '\r' && requestSequence->cstring[iter] == ' '){iter++;}
			while(requestSequence->cstring[iter] != '\0' && requestSequence->cstring[iter] != '\n' && requestSequence->cstring[iter] != '\r' && ((iter == 0) || (iter > 0 && requestSequence->cstring[iter-1] != ')')))
			{
				char_str[0] = requestSequence->cstring[iter];
				append_to_dstring(query_p->value, char_str);
				iter++;
			}
		}
	}

	return 1;
}

void process_query(dstring* responseSequence, query* query_p)
{
	make_dstring_empty(responseSequence);

	int exit_called = 0;

	switch(query_p->command)
	{
		case GET :
		case SET :
		case DEL :
		{
			append_to_dstring(responseSequence, "SUCCESS");
		}
		case ERR :
		default :
		{
			append_to_dstring(responseSequence, "FAILURE");
			break;
		}
	}

	append_to_dstring(responseSequence, "\r\n");

	deinit_dstring(&(query_p->key), "", 0);
	deinit_dstring(&(query_p->value), "", 0);
}