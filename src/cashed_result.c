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
	init_empty_dstring(&(result_p->data), 0);
}

void serialize_result(dstring* str, c_result* result_p)
{
	snprintf_dstring(str, "%d", result_p->code);

	if(!is_empty_dstring(&(result_p->data)))
		snprintf_dstring(str, ":" printf_dstring_format, printf_dstring_params(&(result_p->data)));
	
	snprintf_dstring(str, ";\r\n");
}

void deserialize_result(dstring* str, c_result* result_p)
{
	result_p->code = ((int)(get_byte_array_dstring(str)[0]-'0'));

	if(str->bytes_occupied >= 2 && get_byte_array_dstring(str)[1] == ':')
	{
		int iter_start = 2;
		int count = 0;
		while(get_byte_array_dstring(str)[iter_start + count] != ';' && iter_start + count < get_char_count_dstring(str)){count++;}
		concatenate_dstring(&(result_p->data), &get_literal_dstring(get_byte_array_dstring(str) + 2, count));
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