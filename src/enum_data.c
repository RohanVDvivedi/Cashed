#include<enum_data.h>

Enumeration* get_new_Enumeration(char* enum_type)
{
	Enumeration* enum_data_p = (Enumeration*)malloc(sizeof(Enumeration));
	enum_data_p->enum_type = get_dstring(enum_type, 0);
	enum_data_p->enum_value = 0;
	return enum_data_p;
}

void delete_Enumeration(Enumeration* enum_data_p)
{
	delete_dstring(enum_data_p->enum_type);
	free(enum_data_p);
}