#ifndef RESULT_H
#define RESULT_H

#include<dstring.h>

typedef enum response_code response_code;
enum response_code
{
	FAILURE = 0,
	SUCCESS = 1,
	ERROR_QUERY_NAME = 2,
	ERROR_PARAM_COUNT = 3
};

typedef struct result result;
struct result
{
	response_code code;
	dstring data;
};

void init_result(result* result_p);

void serialize_result(dstring* str, result* result_p);

void deserialize_result(dstring* str, result* result_p);

void print_result(result* result_p);

void deinit_result(result* result_p);

#endif