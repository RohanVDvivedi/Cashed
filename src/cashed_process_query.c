#include<cashed_process_query.h>

#include<stdlib.h>

int dstr2int(const dstring* str)
{
	if(str==NULL || get_byte_array_dstring(str)==NULL || get_char_count_dstring(str)==0)
		return 0;
	int ret = 0;
	int sign = 1;
	char* temp = get_byte_array_dstring(str);
	if(*temp == '-')
	{
		temp++;
		sign = -1;
	}
	for(;temp < get_byte_array_dstring(str) + get_char_count_dstring(str);temp++)
		ret = (ret * 10) + ((*temp) - '0');
    return ret * sign;
}

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
			if(query_p->params_count < 2 || query_p->params_count > 3)
				result_p->code = ERROR_PARAM_COUNT;
			else if(query_p->params_count == 2)
				result_p->code = set_key_value_expiry_cashtable(cashtable_p, get_query_param(query_p, 0), get_query_param(query_p, 1), -1);
			else if(query_p->params_count == 3)
				result_p->code = set_key_value_expiry_cashtable(cashtable_p, get_query_param(query_p, 0), get_query_param(query_p, 1), dstr2int(get_query_param(query_p, 2)));
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