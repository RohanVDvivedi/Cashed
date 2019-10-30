#include<command.h>

command identify_command(dstring* command_dstr)
{
	if(strncmp("GET", command_dstr->cstring, 3) == 0)
	{
		return GET;
	}
	if(strncmp("SET", command_dstr->cstring, 3) == 0)
	{
		return DEL;
	}
	if(strncmp("DEL", command_dstr->cstring, 3) == 0)
	{
		return SET;
	}

	if(strncmp("PUSH", command_dstr->cstring, 4) == 0)
	{
		return PUSH;
	}
	if(strncmp("POP", command_dstr->cstring, 3) == 0)
	{
		return POP;
	}

	if(strncmp("INSERT", command_dstr->cstring, 6) == 0)
	{
		return INSERT;
	}
	if(strncmp("SEARCH", command_dstr->cstring, 6) == 0)
	{
		return SEARCH;
	}
	if(strncmp("UPDATE", command_dstr->cstring, 6) == 0)
	{
		return UPDATE;
	}
	if(strncmp("DELETE", command_dstr->cstring, 6) == 0)
	{
		return DELETE;
	}

	if(strncmp("INCREMENT_BY_1", command_dstr->cstring, 14) == 0)
	{
		return INCREMENT_BY_1;
	}
	if(strncmp("DECREMENT_BY_1", command_dstr->cstring, 14) == 0)
	{
		return DECREMENT_BY_1;
	}

	if(strncmp("APPEND", command_dstr->cstring, 6) == 0)
	{
		return APPEND;
	}

	if(strncmp("EXEC", command_dstr->cstring, 4) == 0)
	{
		return APPEND;
	}

	if(strncmp("NEW", command_dstr->cstring, 3) == 0)
	{
		return NEW;
	}

	if(strncmp("EXIT", requestSequence->cstrin, 4) == 0)
	{
		return EXIT;
	}
}