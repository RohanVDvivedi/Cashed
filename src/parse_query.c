#include<query.h>

parameter* get_query_parameter(query* query_p)
{
	return get_parameter(QUERY, query_p);
}

parameter* get_literal_parameter(dstring* literal_p)
{
	return get_parameter(LITERAL, literal_p);
}

query* parse_query(dstring* sequence)
{
	int state = 0;
	/*
		state = 
		0 => leading space
		1 => query_name
		2 => query name and params starting space
		3 => parsing params
		4 => trailing space after parameters
		5 => query completed by ';'
	*/

	// todo
	return NULL;
}

query* parse_parameters_for_query_name(dstring* sequence, unsigned long long int* parsed_till_index, dstring* query_name)
{
	int state = 0;
	/*
		state = 
		0 => parse parameter
		1 => if ',' goto 1 else else if ')' go to 2
		2 => parsing parameters for query completed
	*/

	// todo
	return NULL;
}

parameter* parse_parameter(dstring* sequence, unsigned long long int* parsed_till_index)
{
	int state = 0;
	/*
		state = 
		0 => leading space
		1 => parameter_name
		2 => trailing space -> if '(' then 3 else if ',' go to 4 
		3 => parsing params
		4 => reading params completed
	*/

	return NULL;
}