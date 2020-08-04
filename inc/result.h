#ifndef RESULT_H
#define RESULT_H

#include<dstring.h>

typedef struct result result;
struct result
{
	int success;
	dstring data;
};

void init_result(result* result_p);

void serialize_result(dstring* str, result* result_p);

void deserialize_result(dstring* str, result* result_p);

void deinit_result(result* result_p);

#endif