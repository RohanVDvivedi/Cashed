#include<process_query.h>

void process_query(result* result_p, query* query_p)
{
	switch(query_p->cmd)
	{
		case GET :
		case SET :
		case DEL :
		{
			//append_to_dstring(responseString, "1");
			break;
		}
		case ERR :
		default :
		{
			//append_to_dstring(responseString, "0");
			break;
		}
	}

	//append_to_dstring(responseString, ";\r\n");

	deinit_dstring(&(query_p->key));
	deinit_dstring(&(query_p->value));
}