#include<query.h>

query* get_query(dstring* command_or_datatype_name)
{
	query* query_p = (query*) malloc(sizeof(query));
	query_p->command_or_datatype_name = command_or_datatype_name;
	query_p->parameter_count = 0;
	query_p->parameters = get_array(10);
	return query_p;
}

parameter* get_parameter(parameter_type type, void* value)
{
	parameter* parameter_p = (parameter*) malloc(sizeof(parameter));
	parameter_p->type = type;
	parameter_p->value = value;
	return parameter_p;
}

void insert_parameter_for_query(query* query_p, parameter* parameter_p)
{
	if(query_p->parameter_count + 1 >= query_p->parameters->total_size)
	{
		expand_array(query_p->parameters);
	}
	set_element(query_p->parameters, parameter_p, query_p->parameter_count++);
}

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