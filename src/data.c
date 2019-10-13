#include<data.h>

Data* get_new_data(TypeOfData type, dstring* init_params)
{
	Data* data_p = (Data*)malloc(sizeof(Data));
	data_p->type = type;
	data_p->rwL  = get_rwlock();
	data_p->value = NULL;
	switch(type)
	{
		case NUM_DECIMAL :
		case STRING :
		{
			data_p->value = get_dstring("", 10);
			break;
		}
		case NUM_FLOAT :
		{
			data_p->value = malloc(sizeof(double));
			break;
		}
		case NUM_INTEGER :
		{
			data_p->value = malloc(sizeof(long long int));
			break;
		}
		case TIME_STAMP :
		{
			data_p->value = malloc(sizeof(unsigned long long int));
			(*(data_p->value)) = 0;
			break;
		}
		case ENUMERATION :
		{
			data_p->value = get_new_Enumeration("");
			break;
		}
		default :
		{
			break;
		}
	}
	return data_p;
}

void delete_simple_data(Data* data_p)
{
	switch(type)
	{
		case NUM_DECIMAL :
		case STRING :
		{
			delete_dstring(data_p->value);
			break;
		}
		case NUM_FLOAT :
		case NUM_INTEGER :
		case TIME_STAMP :
		{
			free(data_p->value);
			break;
		}
		case ENUMERATION :
		{
			delete_Enumeration(data_p->value);
			break;
		}
		default :
		{
			break;
		}
	}
	free(data_p);
}