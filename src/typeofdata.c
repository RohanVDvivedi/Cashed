#include<typeofdata.h>

TypeOfData get_type_from_dstring(dstring* type)
{
	if(strcmp("STRING", type->cstring) == 0)
	{return STRING;}

	if(strcmp("NUM_DECIMAL", type->cstring) == 0)
	{return NUM_DECIMAL;}

	if(strcmp("NUM_FLOAT", type->cstring) == 0)
	{return NUM_FLOAT;}

	if(strcmp("NUM_INTEGER", type->cstring) == 0)
	{return NUM_INTEGER;}

	if(strcmp("TIME_STAMP", type->cstring) == 0)
	{return TIME_STAMP;}

	return UNIDENTIFIED;
}

void serialize_type(dstring* type_dstring, TypeOfData type)
{
	switch(type)
	{
		case STRING :
		{
			append_to_dstring(type_dstring, "STRING");
			break;
		}
		case NUM_DECIMAL :
		{
			append_to_dstring(type_dstring, "NUM_DECIMAL");
			break;
		}
		case NUM_FLOAT :
		{
			append_to_dstring(type_dstring, "NUM_FLOAT");
			break;
		}
		case NUM_INTEGER :
		{
			append_to_dstring(type_dstring, "NUM_INTEGER");
			break;
		}
		case TIME_STAMP :
		{
			append_to_dstring(type_dstring, "TIME_STAMP");
			break;
		}
		case UNIDENTIFIED :
		default :
		{
			append_to_dstring(type_dstring, "UNIDENTIFIED");
			break;
		}
	}
	return;
}