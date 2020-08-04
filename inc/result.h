#ifndef RESULT_H
#define RESULT_H

typedef struct result result;
struct result
{
	int success;
	dstring data;
};

void init_result(query* query_p);

void serialize_result(dstring* str, query* query_p);

void deserialize_result(dstring* str, query* query_p);

void deinit_result(query* query_p);

#endif