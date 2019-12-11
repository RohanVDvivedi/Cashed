#include<data.h>
#include<jenkinshash.h>

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
	if(strcmp("TIME_STAMP", type->cstring) == 0 || 
		strcmp("NOW", type->cstring) == 0)
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

Data* get_new_data(TypeOfData type, dstring* input_param)
{
	Data* data_p = (Data*)malloc(sizeof(Data));
	data_p->type = type;
	data_p->rwL  = get_rwlock();
	data_p->value = NULL;
	switch(type)
	{
		case STRING :
		case NUM_DECIMAL :
		{
			data_p->value = get_dstring("", 0);
			concatenate_dstring(data_p->value, input_param);
			break;
		}
		case NUM_FLOAT :
		{
			data_p->value = malloc(sizeof(double));
			sscanf(input_param->cstring, "%lf", ((double*)(data_p->value)));
			break;
		}
		case NUM_INTEGER :
		{
			data_p->value = malloc(sizeof(long long int));
			sscanf(input_param->cstring, "%lld", ((long long int*)(data_p->value)));
			break;
		}
		case TIME_STAMP :
		{
			data_p->value = malloc(sizeof(unsigned long long int));
			if(strcmp("NOW", input_param->cstring) == 0)
			{
				data_p->value = malloc(sizeof(unsigned long long int));
				(*((unsigned long long int*)(data_p->value))) = time(NULL);
			}
			else
			{
				char str[50];
				struct tm tim;
				sscanf(input_param->cstring, "%s", str);
				SCAN_TIME_STAMP(str, tim);
				(*((unsigned long long int*)(data_p->value))) = (unsigned long long int)mktime(&tim);
			}
			break;
		}
		default :
		{

		}
	}
	return data_p;
}

void transfer_data(Data* destination, Data* source)
{
	delete_data_contents(destination);
	(*destination) = (*source);
	source->value = NULL;
}

int compare_data(const Data* data_p1, const Data* data_p2)
{
	if(data_p1->type != data_p2->type)
	{
		return data_p1->type - data_p2->type;
	}
	switch(data_p1->type)
	{
		case NUM_DECIMAL :
		{
			return compare_decimal(((dstring*)(data_p1->value)), ((dstring*)(data_p2->value)));
		}
		case STRING :
		{
			return compare_dstring(((dstring*)(data_p1->value)), ((dstring*)(data_p2->value)));
		}
		case NUM_FLOAT :
		{
			return (*((double*)(data_p1->value))) - (*((double*)(data_p2->value)));
		}
		case NUM_INTEGER :
		{
			return (*((long long int*)(data_p1->value))) - (*((long long int*)(data_p2->value)));
		}
		case TIME_STAMP :
		{
			return (*((unsigned long long int*)(data_p1->value))) - (*((unsigned long long int*)(data_p2->value)));
		}
		default :
		{
			return 0;
		}
	}
}

unsigned long long int hash_data(const Data* data_p)
{
	switch(data_p->type)
	{
		case NUM_DECIMAL :
		case STRING :
		{
			dstring* data_value = ((dstring*)(data_p->value));
			return jenkins_hash(data_value->cstring, data_value->bytes_occupied);
		}
		case NUM_FLOAT :
		{
			return jenkins_hash(data_p->value, sizeof(double));
		}
		case NUM_INTEGER :
		{
			return jenkins_hash(data_p->value, sizeof(long long int));
		}
		case TIME_STAMP :
		{
			return jenkins_hash(data_p->value, sizeof(unsigned long long int));
		}
		default :
		{
			return 0;
		}
	}
}

void serialize_data(dstring* destination, const Data* data_p)
{
	if(data_p == NULL)
	{
		append_to_dstring(destination, "NULL");
	}
	else
	{
		switch(data_p->type)
		{
			case NUM_DECIMAL :
			{
				append_to_dstring(destination, "NUM_DECIMAL(");
				concatenate_dstring(destination, ((dstring*)(data_p->value)));
				append_to_dstring(destination, ")");
				break;
			}
			case STRING :
			{
				append_to_dstring(destination, "STRING(");
				concatenate_dstring(destination, ((dstring*)(data_p->value)));
				append_to_dstring(destination, ")");
				break;
			}
			case NUM_FLOAT :
			{
				char num[50];sprintf(num, "NUM_FLOAT(%lf)", (*((double*)(data_p->value))));
				append_to_dstring(destination, num);
				break;
			}
			case NUM_INTEGER :
			{
				char num[50];sprintf(num, "NUM_INTEGER(%lld)", (*((long long int*)(data_p->value))));
				append_to_dstring(destination, num);
				break;
			}
			case TIME_STAMP :
			{
				char time_as_formatted_string[50] = "";
				struct tm* tm_standard_p = localtime((time_t*)((unsigned long long int*)(data_p->value)));
				strftime(time_as_formatted_string, 49, TIME_STAMP_FORMAT, tm_standard_p);
				char num[50];sprintf(num, "TIME_STAMP(%s)", time_as_formatted_string);
				append_to_dstring(destination, num);
				break;
			}
			default :
			{
				append_to_dstring(destination, "UNIDENTIFIED");
				break;
			}
		}
	}
}

void delete_data_contents(Data* data_p)
{
	if(data_p->value != NULL)
	{
		switch(data_p->type)
		{
			case NUM_DECIMAL :
			case STRING :
			{
				delete_dstring(((dstring*)(data_p->value)));
				break;
			}
			case NUM_FLOAT :
			case NUM_INTEGER :
			case TIME_STAMP :
			{
				free(data_p->value);
				break;
			}
			default :
			{
				break;
			}
		}
	}
	data_p->value = NULL;
}

void delete_data(Data* data_p)
{
	delete_data_contents(data_p);
	delete_rwlock(data_p->rwL);
	free(data_p);
}