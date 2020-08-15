#include<query.h>

#include<string.h>

#include<stdio.h>

void init_query(query* query_p)
{
	query_p->cmd = ERR;
	query_p->params_count = 0;
	initialize_array(&(query_p->params), 3);
}

void serialize_query(dstring* str, query* query_p)
{
	serialize_command(query_p->cmd, str);
	append_to_dstring(str, "(");
	for(unsigned int i = 0; i < query_p->params_count; i++)
	{
		if(i > 0)
			append_to_dstring(str, ",");
		append_to_dstring(str, ((dstring*)get_element(&(query_p->params), i))->cstring);
	}
	append_to_dstring(str, ");\r\n");
}

void add_query_param(query* query_p, dstring* new_param)
{
	if(query_p->params_count == query_p->params.total_size)
		expand_array(&(query_p->params));
	set_element(&(query_p->params), new_param, query_p->params_count++);
}

const dstring* get_query_param(const query* query_p, unsigned int index)
{
	return get_element(&(query_p->params), index);
}

void deserialize_query(dstring* str, query* query_p)
{
	int iter = 0;

	query_p->cmd = deserialize_command(str);

	if(query_p->cmd == ERR)
		return;

	iter += strlen(command_strings[query_p->cmd]);

	while(str->cstring[iter] != '(' && str->cstring[iter] != '\0'){iter++;}

	while(str->cstring[iter] != ')' && str->cstring[iter] != '\0')
	{
		int start = iter + 1;
		iter++;
		while(str->cstring[iter] != ',' && str->cstring[iter] != ')' && str->cstring[iter] != '\0'){iter++;}
		int end = iter - 1;

		dstring* new_param = get_dstring("", 0);
		appendn_to_dstring(new_param, str->cstring + start, end - start + 1);

		add_query_param(query_p, new_param);
	}
}

void print_query(query* query_p)
{
	printf("%s\n", command_strings[query_p->cmd]);
	printf("params_count = %u\n", query_p->params_count);
	for(unsigned int i = 0; i < query_p->params_count; i++)
	{
		printf("\t");
		display_dstring(((dstring*)get_element(&(query_p->params), i)));
		printf("\n");
	}
}

void deinit_query(query* query_p)
{
	for(unsigned int i = 0; i < query_p->params_count; i++)
		delete_dstring(((dstring*)get_element(&(query_p->params), i)));
	deinitialize_array(&(query_p->params));
}