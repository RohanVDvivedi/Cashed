#ifndef ENUM_DATA_H
#define ENUM_DATA_H

#include<dstring.h>

typedef struct Enumeration Enumeration;
struct Enumeration
{
	dstring* enum_type;
	uint8_t enum_value;
};

Enumeration* get_new_Enumeration();

void delete_Enumeration(Enumeration* enum_data_p);

#endif