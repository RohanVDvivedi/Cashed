#include<process_query.h>

void process_query(result* result_p, query* query_p)
{
	print_query(query_p);

	switch(query_p->cmd)
	{
		case GET :
		{
			if(query_p->params_count != 1)
				result_p->success = 0;
			else
				result_p->success = get_hashtable(NULL, get_query_param(query_p, 0), &(result_p->data));
			break;
		}
		case SET :
		{
			if(query_p->params_count != 2)
				result_p->success = 0;
			else
				result_p->success = set_hashtable(NULL, get_query_param(query_p, 0), get_query_param(query_p, 1));
			break;
		}
		case DEL :
		{
			if(query_p->params_count != 1)
				result_p->success = 0;
			else
				result_p->success = del_hashtable(NULL, get_query_param(query_p, 0));
			break;
		}
		case ERR :
		default :
		{
			result_p->success = 0;
			break;
		}
	}
}