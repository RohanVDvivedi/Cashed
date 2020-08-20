#ifndef RESULT_H
#define RESULT_H

#include<dstring.h>

typedef enum c_result_code c_result_code;
enum c_result_code
{
	FAILURE = 0,
	SUCCESS = 1,
	ERROR_QUERY_NAME = 2,
	ERROR_PARAM_COUNT = 3
};

typedef struct c_result c_result;
struct c_result
{
	c_result_code code;
	dstring data;
};

void init_result(c_result* result_p);

void serialize_result(dstring* str, c_result* result_p);

void deserialize_result(dstring* str, c_result* result_p);

void print_result(c_result* result_p);

void deinit_result(c_result* result_p);

#endif