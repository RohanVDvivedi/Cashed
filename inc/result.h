#ifndef RESULT_H
#define RESULT_H

typedef struct result result;
struct result
{
	int success;
	dstring data;
};

#endif