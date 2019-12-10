#include<command.h>

command identify_command(dstring* command_dstr)
{
	if(strcmp("GET", command_dstr->cstring) == 0)
	{
		return GET;
	}
	if(strcmp("SET", command_dstr->cstring) == 0)
	{
		return SET;
	}
	if(strcmp("DEL", command_dstr->cstring) == 0)
	{
		return DEL;
	}

	if(strcmp("PUSH", command_dstr->cstring) == 0)
	{
		return PUSH;
	}
	if(strcmp("POP", command_dstr->cstring) == 0)
	{
		return POP;
	}

	if(strcmp("INSERT", command_dstr->cstring) == 0)
	{
		return INSERT;
	}
	if(strcmp("SEARCH", command_dstr->cstring) == 0)
	{
		return SEARCH;
	}
	if(strcmp("UPDATE", command_dstr->cstring) == 0)
	{
		return UPDATE;
	}
	if(strcmp("DELETE", command_dstr->cstring) == 0)
	{
		return DELETE;
	}

	if(strcmp("INCREMENT_BY_1", command_dstr->cstring) == 0)
	{
		return INCREMENT_BY_1;
	}
	if(strcmp("DECREMENT_BY_1", command_dstr->cstring) == 0)
	{
		return DECREMENT_BY_1;
	}

	if(strcmp("APPEND", command_dstr->cstring) == 0)
	{
		return APPEND;
	}

	if(strcmp("EXEC", command_dstr->cstring) == 0)
	{
		return EXEC;
	}

	if(strcmp("NEW", command_dstr->cstring) == 0)
	{
		return NEW;
	}

	if(strcmp("EXIT", command_dstr->cstring) == 0)
	{
		return EXIT;
	}

	return ERROR;
}

Data* SEARCH_command(Data* data_structure, Data* key)
{
	Data* result = NULL;
	switch(data_structure->type)
	{
		case HASHTABLE :
		{
			result = (Data*)(find_value_from_hash((hashmap*)(data_structure->value), key));
			break;
		}
	}
	return result;
}

void INSERT_command(Data* data_structure, Data* key, Data* value)
{
	switch(data_structure->type)
	{
		case HASHTABLE :
		{
			insert_entry_in_hash((hashmap*)(data_structure->value), key, value);
			break;
		}
	}
}

int UPDATE_command(Data* data_structure, Data* key, Data* value)
{
	Data* value_old = GET_command(data_structure, key);
	if(value_old != NULL)
	{
		transfer_data(value_old, value);
		return 1;
	}
}

int DELETE_command(Data* data_structure, Data* key, Data** return_key, Data** return_value)
{
	int elements_deleted = 0;
	switch(data_structure->type)
	{
		case HASHTABLE :
		{
			elements_deleted = delete_entry_from_hash((hashmap*)(data_structure->value), key, (const void**)return_key, (const void**)return_value);
			break;
		}
	}
	return elements_deleted;
}

Data* GET_command(Data* data_structure, Data* key)
{
	return SEARCH_command(data_structure, key);
}

int SET_command(Data* data_structure, Data* key, Data* value)
{
	Data* value_old = GET_command(data_structure, key);
	if(value_old == NULL)
	{
		INSERT_command(data_structure, key, value);
	}
	else
	{
		transfer_data(value_old, value);
		return 1;
	}
	return 0;
}

int DEL_command(Data* data_structure, Data* key, Data** return_key, Data** return_value)
{
	return DELETE_command(data_structure, key, return_key, return_value);
}