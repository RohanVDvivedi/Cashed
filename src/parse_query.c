#include<query.h>

query* parse_parameters_for_query_name(dstring* sequence, unsigned long long int* parsed_till_index, dstring* query_name);

parameter* parse_parameter(dstring* sequence, unsigned long long int* parsed_till_index)
{
	int state = 0;
	/*
		parses : < hello >
		parses : < hello ( <parameters> ) >

		state = 
		0 => leading space
		1 => parameter_name
		2 => trailing space 
		3 => param name complete -> if '(' then 4 else if ',' or ')' go to 5
		4 => parsing params
		5 => reading params completed

		6 => error
	*/

	dstring* parameter_name = get_dstring("", 10);
	parameter* result = NULL;

	while(state != 5 && sequence->cstring[(*parsed_till_index)] != '\0')
	{
		switch(state)
		{
			case 0 :
			case 2 :
			{
				if(sequence->cstring[(*parsed_till_index)] != ' ' && sequence->cstring[(*parsed_till_index)] != '\t' && sequence->cstring[(*parsed_till_index)] != '\n')
				{
					state += 1;
				}
				else
				{
					(*parsed_till_index)++;
				}
				break;
			}
			case 1 :
			{
				if(sequence->cstring[(*parsed_till_index)] != ' ' && sequence->cstring[(*parsed_till_index)] != '\t' && sequence->cstring[(*parsed_till_index)] != '\n'
					&& sequence->cstring[(*parsed_till_index)] != '(' 
					&& sequence->cstring[(*parsed_till_index)] != ','
					&& sequence->cstring[(*parsed_till_index)] != ')')
				{
					char temp[2] = "";
					temp[0] = sequence->cstring[(*parsed_till_index)];
					temp[1] = '\0';
					append_to_dstring(parameter_name, temp);
					(*parsed_till_index)++;
				}
				else
				{
					state = 3;
				}
				break;
			}
			case 3 :
			{
				if(sequence->cstring[(*parsed_till_index)] == '(')
				{
					(*parsed_till_index)++;
					state = 4;
				}
				else if(sequence->cstring[(*parsed_till_index)] == ',' || sequence->cstring[(*parsed_till_index)] == ')')
				{
					result = get_parameter(LITERAL, parameter_name);
					state = 5;
				}
				else
				{
					state = 6;
				}
				break;
			}
			case 4 :
			{
				query* query_p = parse_parameters_for_query_name(sequence, parsed_till_index, parameter_name);
				if(query_p != NULL)
				{
					result = get_parameter(QUERY, query_p);
					state = 5;
				}
				else
				{
					state = 6;
				}
				break;
			}
			default :
			{
				state = 6;
				break;
			}
		}

		if(state == 6)
		{
			break;
		}

	}

	return result;
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
		5 => query completed
		6 => encounter statement completion by ';'

		7 => error
	*/

	query* result = NULL;

	unsigned long long int parsed_till_index = 0;

	dstring* query_name = get_dstring("", 10);

	while(state != 6 && sequence->cstring[parsed_till_index] != '\0')
	{
		switch(state)
		{
			case 0 :
			case 2 :
			case 4 :
			{
				if(sequence->cstring[parsed_till_index] != ' ' && sequence->cstring[parsed_till_index] != '\t' && sequence->cstring[parsed_till_index] != '\n')
				{
					state += 1;
				}
				else
				{
					parsed_till_index++;
				}
				break;
			}
			case 1 :
			{
				if(sequence->cstring[parsed_till_index] != ' ' && sequence->cstring[parsed_till_index] != '\t' && sequence->cstring[parsed_till_index] != '\n'
					&& sequence->cstring[parsed_till_index] != '(' 
					&& sequence->cstring[parsed_till_index] != ';')
				{
					char temp[2] = "";
					temp[0] = sequence->cstring[parsed_till_index];
					temp[1] = '\0';
					append_to_dstring(query_name, temp);
					parsed_till_index++;
				}
				else if(sequence->cstring[parsed_till_index] == '(')
				{
					state = 3;
				}
				else if(sequence->cstring[parsed_till_index] == ';')
				{
					delete_dstring(query_name); query_name = NULL;
					state = 7;
				}
				else
				{
					state = 2;
				}
				break;
			}
			case 3 :
			{
				if(sequence->cstring[parsed_till_index] == '(')
				{
					parsed_till_index++;
				}
				else
				{
					delete_dstring(query_name); query_name = NULL;
					state = 7;
					break;
				}
				result = parse_parameters_for_query_name(sequence, &parsed_till_index, query_name);
				if(result == NULL)
				{
					state = 7;
				}
				else if(sequence->cstring[parsed_till_index] == ';')
				{
					parsed_till_index++;
					state = 6;
				}
				else if(sequence->cstring[parsed_till_index] == ' ' && sequence->cstring[parsed_till_index] == '\t' && sequence->cstring[parsed_till_index] == '\n')
				{
					state = 4;
				}
				else
				{
					delete_query(result); result = NULL; query_name = NULL;
					state = 7;
				}
				break;
			}
			case 5 :
			{
				if(sequence->cstring[parsed_till_index] == ';')
				{
					state = 6;
				}
				else
				{
					delete_query(result); result = NULL; query_name = NULL;
					state = 7;
				}
				break;
			}
			default :
			{
				state = 7;
				break;
			}
		}

		if(state == 7)
		{
			break;
		}
	}

	return result;
}

query* parse_parameters_for_query_name(dstring* sequence, unsigned long long int* parsed_till_index, dstring* query_name)
{
	int state = 0;
	/*
		state = 
		0 => leading space
		1 => parse parameter
		2 => trailing space
		3 => if ',' goto 0 else else if ')' go to 4
		4 => parsing parameters for query completed

		5 => error
	*/

	query* result = get_query(query_name);

	while(state != 4 && sequence->cstring[(*parsed_till_index)] != '\0')
	{
		switch(state)
		{
			case 0 :
			case 2 :
			{
				if(sequence->cstring[(*parsed_till_index)] != ' ' && sequence->cstring[(*parsed_till_index)] != '\t' && sequence->cstring[(*parsed_till_index)] != '\n')
				{
					state += 1;
				}
				else
				{
					(*parsed_till_index)++;
				}
				break;
			}
			case 1 :
			{
				parameter* parameter_p = parse_parameter(sequence, parsed_till_index);
				if(parameter_p != NULL)
				{
					insert_parameter_for_query(result, parameter_p);
					state = 2;
				}
				else
				{
					state = 5;
				}
				break;
			}
			case 3 :
			{
				if(sequence->cstring[(*parsed_till_index)] == ',')
				{
					(*parsed_till_index)++;
					state = 0;
				}
				else if(sequence->cstring[(*parsed_till_index)] == ')')
				{
					(*parsed_till_index)++;
					state = 4;
				}
				else
				{
					state = 5;
				}
				break;
			}
			default :
			{
				state = 5;
				break;
			}
		}

		if(state == 5)
		{
			delete_query(result);
			break;
		}
	}

	return result;
}
