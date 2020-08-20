#include<cashed_result.h>

#include<stdio.h>

char* response_code_strings[] = {
									"FAILURE", 
									"SUCCESS",
									"ERROR_QUERY_NAME",
									"ERROR_PARAM_COUNT"
								};

void init_result(result* result_p)
{
	result_p->code = 0;
	init_dstring(&(result_p->data), "", 0);
}

void serialize_result(dstring* str, result* result_p)
{
	char num_str[30];
	sprintf(num_str, "%d", result_p->code);
	append_to_dstring(str, num_str);

	if(result_p->data.cstring[0] != '\0')
	{
		append_to_dstring(str, ":");
		append_to_dstring(str, result_p->data.cstring);
	}
	
	append_to_dstring(str, ";\r\n");
}

void deserialize_result(dstring* str, result* result_p)
{
	result_p->code = ((int)(str->cstring[0]-'0'));

	if(str->cstring[1] == ':')
	{
		int iter_start = 2;
		int count = 0;
		while(str->cstring[iter_start + count] != ';' && str->cstring[iter_start + count] != '\0'){count++;}
		appendn_to_dstring(&(result_p->data), str->cstring + 2, count);
	}
}

void print_result(result* result_p)
{
	printf("code : %s\n", response_code_strings[result_p->code]);
	printf("data : ");display_dstring(&(result_p->data));printf("\n");
}

void deinit_result(result* result_p)
{
	deinit_dstring(&(result_p->data));
}