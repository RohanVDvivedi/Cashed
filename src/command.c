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