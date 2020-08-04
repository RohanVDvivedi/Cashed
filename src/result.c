#include<result.h>

void init_result(result* result_p)
{
	result_p->success = 0;
	init_dstring(&(result_p->data), "", 0);
}

void serialize_result(dstring* str, result* result_p)
{
	if(result_p->success)
		append_to_dstring(str, "1");
	else
		append_to_dstring(str, "0");

	if(result_p->data.cstring[0] != '\0')
	{
		append_to_dstring(str, ":");
		append_to_dstring(str, result_p->data.cstring);
	}
	
	append_to_dstring(str, ";");
}

void deserialize_result(dstring* str, result* result_p)
{

}

void deinit_result(result* result_p)
{
	deinit_dstring(&(result_p->data));
}