#include<result.h>

void init_result(result* result_p)
{
	result_p->success = 0;
	init_dstring(&(result_p->data), "", 0);
}

void serialize_result(dstring* str, result* result_p)
{

}

void deserialize_result(dstring* str, result* result_p)
{

}

void deinit_result(result* result_p)
{
	deinit_dstring(&(result_p->data));
}