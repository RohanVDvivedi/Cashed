#include<data.h>
#include<jenkinshash.h>

Data* get_new_data(TypeOfData type, void* init_params)
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
			data_p->value = (void*)get_dstring(((char*)init_params), 0);
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
			break;
		}
		case ENUMERATION :
		{
			data_p->value = (void*)get_new_Enumeration(((char*)init_params));
			break;
		}
		default :
		{
			break;
		}
	}
	return data_p;
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
		case ENUMERATION :
		{
			return (*((uint8_t*)(data_p1->value))) - (*((uint8_t*)(data_p2->value)));
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
			return jenkins_hash(((dstring*)data_p)->cstring, ((dstring*)data_p)->bytes_occupied);
		}
		case NUM_FLOAT :
		{
			return jenkins_hash(data_p, sizeof(double));
		}
		case NUM_INTEGER :
		{
			return jenkins_hash(data_p, sizeof(long long int));
		}
		case TIME_STAMP :
		{
			return jenkins_hash(data_p, sizeof(unsigned long long int));
		}
		case ENUMERATION :
		{
			return jenkins_hash(data_p, sizeof(uint8_t));
		}
		default :
		{
			return 0;
		}
	}
}

void delete_simple_data(Data* data_p)
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
		case ENUMERATION :
		{
			delete_Enumeration(((Enumeration*)(data_p->value)));
			break;
		}
		default :
		{
			break;
		}
	}
	get_rwlock(data_p->rwL);
	free(data_p);
}