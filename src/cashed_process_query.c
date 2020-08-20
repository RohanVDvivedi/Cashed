#include<cashed_process_query.h>

void process_query(cashtable* cashtable_p, c_query* query_p, c_result* result_p)
{
	switch(query_p->cmd)
	{
		case GET :
		{
			if(query_p->params_count != 1)
				result_p->code = ERROR_PARAM_COUNT;
			else
				result_p->code = get_value_cashtable(cashtable_p, get_query_param(query_p, 0), &(result_p->data));
			break;
		}
		case SET :
		{
			if(query_p->params_count != 2)
				result_p->code = ERROR_PARAM_COUNT;
			else
				result_p->code = set_key_value_cashtable(cashtable_p, get_query_param(query_p, 0), get_query_param(query_p, 1));
			break;
		}
		case DEL :
		{
			if(query_p->params_count != 1)
				result_p->code = ERROR_PARAM_COUNT;
			else
				result_p->code = del_key_value_cashtable(cashtable_p, get_query_param(query_p, 0));
			break;
		}
		case ERR :
		default :
		{
			result_p->code = ERROR_QUERY_NAME;
			break;
		}
	}
}