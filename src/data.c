#include<data.h>
#include<jenkinshash.h>

Data* get_new_data(dstring* serialized_data)
{
	Data* data_p = (Data*)malloc(sizeof(Data));
	data_p->rwL  = get_rwlock();
	data_p->value = NULL;
	if(strncmp("STRING", serialized_data->cstring, 6) == 0)
	{
		data_p->type = STRING;
		data_p->value = get_dstring("", 0);
		appendn_to_dstring(data_p->value, serialized_data->cstring + 7, serialized_data->bytes_occupied-2-7);
	}
	if(strncmp("NUM_DECIMAL", serialized_data->cstring, 11) == 0)
	{
		data_p->type = NUM_DECIMAL;
		data_p->value = get_dstring("", 0);
		appendn_to_dstring(data_p->value, serialized_data->cstring + 11, serialized_data->bytes_occupied-2-11);
	}
	else if(strncmp("NUM_FLOAT", serialized_data->cstring, 9) == 0)
	{
		data_p->type = NUM_FLOAT;
		data_p->value = malloc(sizeof(double));
		sscanf(serialized_data->cstring, "NUM_FLOAT(%lf)", ((double*)(data_p->value)));
	}
	else if(strncmp("NUM_INTEGER", serialized_data->cstring, 11) == 0)
	{
		data_p->type = NUM_INTEGER;
		data_p->value = malloc(sizeof(long long int));
		sscanf(serialized_data->cstring, "NUM_INTEGER(%lld)", ((long long int*)(data_p->value)));
	}
	else if(strncmp("TIME_STAMP", serialized_data->cstring, 10) == 0)
	{
		data_p->type = TIME_STAMP;
		data_p->value = malloc(sizeof(unsigned long long int));
		char str[50];
		struct tm tim;
		sscanf(serialized_data->cstring, "TIME_STAMP(%s)", str);
		SCAN_TIME_STAMP(str, tim);
		(*((unsigned long long int*)(data_p->value))) = (unsigned long long int)mktime(&tim);
	}
	// constructs timestamp with value in seconds
	else if(strncmp("NOW", serialized_data->cstring, 3) == 0)
	{
		data_p->type = TIME_STAMP;
		data_p->value = malloc(sizeof(unsigned long long int));
		(*((unsigned long long int*)(data_p->value))) = time(NULL);
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
	get_rwlock(data_p->rwL);
	free(data_p);
}