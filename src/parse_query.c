#include<query.h>

parameter* get_query_parameter(query* query_p)
{
	return get_parameter(QUERY, query_p);
}

parameter* get_literal_parameter(dstring* literal_p)
{
	return get_parameter(LITERAL, literal_p);
}

query* parse_query(dstring* requestSequence)
{
	// todo
	return NULL;
}