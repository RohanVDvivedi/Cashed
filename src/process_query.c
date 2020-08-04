#include<process_query.h>

void process_query(result* result_p, query* query_p)
{
	print_query(query_p);

	switch(query_p->cmd)
	{
		case GET :
		case SET :
		case DEL :
		{
			result_p->success = 1;
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