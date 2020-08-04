#include<query.h>

void init_query(query* query_p)
{
	query_p->cmd = ERR;
	init_dstring(&(query_p->key), "", 0);
	init_dstring(&(query_p->value), "", 0);
}

void serialize_query(dstring* str, query* query_p)
{
	serialize_command(query_p->cmd, str);
	append_to_dstring(str, "(");
	append_to_dstring(str, query_p->key.cstring);
	if(query_p->cmd == SET)
	{
		append_to_dstring(str, ",");
		append_to_dstring(str, query_p->value.cstring);
	}
	append_to_dstring(str, ");\r\n");
}

void deserialize_query(dstring* str, query* query_p)
{
	int iter = 0;

	query_p->cmd = deserialize_command(str);
	iter += strlen(command_strings[query_p->cmd]);

	if(query_p->cmd != ERR)
	{
		while(str->cstring[iter] != '(' && str->cstring[iter] != '\0'){iter++;}
		int key_start = iter + 1;
		while(str->cstring[++iter] != ',' && str->cstring[iter] != ')' && str->cstring[iter] != '\0'){iter++;}
		int key_end = iter - 1;

		appendn_to_dstring(&(query_p->key), str->cstring + key_start, key_end - key_start + 1);

		if(query_p->cmd == SET)
		{
			int value_start = iter + 1;
			while(str->cstring[iter] != ')' && str->cstring[iter] != '\0'){iter++;}
			int value_end = iter - 1;

			appendn_to_dstring(&(query_p->value), str->cstring + value_start, value_end - value_start + 1);
		}
	}
}

void deinit_query(query* query_p)
{
	deinit_dstring(&(query_p->key));
	deinit_dstring(&(query_p->value));
}