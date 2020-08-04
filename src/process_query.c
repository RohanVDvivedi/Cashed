#include<process_query.h>

void process_query(result* result_p, query* query_p)
{
	printf("key : ");
	display_dstring(&(query_p->key));
	printf("\n");

	printf("value : ");
	display_dstring(&(query_p->value));
	printf("\n");

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