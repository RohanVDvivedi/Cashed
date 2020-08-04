#include<query.h>

void parse_query(dstring* requestString, query* query_p)
{
	query_p->cmd = deserialize_command(requestString);

	if(query_p->cmd != ERR)
	{
		init_dstring(&(query_p->key), "", 0);

		char char_str[2] = "x";
		unsigned long long int iter = 4;

		while(requestString->cstring[iter] != '\0' && requestString->cstring[iter] != '\n' && requestString->cstring[iter] != '\r' && requestString->cstring[iter] == ' '){iter++;}
		while(requestString->cstring[iter] != '\0' && requestString->cstring[iter] != '\n' && requestString->cstring[iter] != '\r' && ((iter == 0) || (iter > 0 && requestString->cstring[iter-1] != ')')))
		{
			char_str[0] = requestString->cstring[iter];
			append_to_dstring(&(query_p->key), char_str);
			iter++;
		}

		if(query_p->cmd == SET)
		{
			init_dstring(&(query_p->value), "", 0);
			while(requestString->cstring[iter] != '\0' && requestString->cstring[iter] != '\n' && requestString->cstring[iter] != '\r' && requestString->cstring[iter] == ' '){iter++;}
			while(requestString->cstring[iter] != '\0' && requestString->cstring[iter] != '\n' && requestString->cstring[iter] != '\r' && ((iter == 0) || (iter > 0 && requestString->cstring[iter-1] != ')')))
			{
				char_str[0] = requestString->cstring[iter];
				append_to_dstring(&(query_p->value), char_str);
				iter++;
			}
		}
	}
}

void process_query(dstring* responseString, query* query_p)
{
	switch(query_p->cmd)
	{
		case GET :
		case SET :
		case DEL :
		{
			append_to_dstring(responseString, "1");
			break;
		}
		case ERR :
		default :
		{
			append_to_dstring(responseString, "0");
			break;
		}
	}

	append_to_dstring(responseString, ";\r\n");

	deinit_dstring(&(query_p->key));
	deinit_dstring(&(query_p->value));
}