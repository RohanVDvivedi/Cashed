#include<cashed_result.h>

#include<stdio.h>

char* c_result_code_strings[] = {
									"FAILURE", 
									"SUCCESS",
									"ERROR_QUERY_NAME",
									"ERROR_PARAM_COUNT"
								};

void init_result(c_result* result_p)
{
	result_p->code = 0;
	init_dstring(&(result_p->data), NULL, 0);
}

void serialize_result(dstring* str, c_result* result_p)
{
	snprintf_dstring(str, "%d", result_p->code);

	if(result_p->data.bytes_occupied != 0)
	{
		snprintf_dstring(str, ":");
		concatenate_dstring(str, &(result_p->data));
	}
	
	snprintf_dstring(str, ";\r\n");
}

void deserialize_result(dstring* str, c_result* result_p)
{
	result_p->code = ((int)(str->cstring[0]-'0'));

	if(str->bytes_occupied >= 2 && str->cstring[1] == ':')
	{
		int iter_start = 2;
		int count = 0;
		while(str->cstring[iter_start + count] != ';' && iter_start + count < str->bytes_occupied){count++;}
		concatenate_dstring(&(result_p->data), dstring_DUMMY_DATA(str->cstring + 2, count));
	}
}

void print_result(c_result* result_p)
{
	printf("code : %s\n", c_result_code_strings[result_p->code]);
	printf("data : ");printf_dstring(&(result_p->data));printf("\n");
}

void deinit_result(c_result* result_p)
{
	deinit_dstring(&(result_p->data));
}