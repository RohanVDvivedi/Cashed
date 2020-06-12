#include<command.h>

command identify_command(dstring* command_dstr)
{
	if(strncmp("GET", command_dstr->cstring, 3) == 0)
		return GET;
	if(strncmp("SET", command_dstr->cstring, 3) == 0)
		return SET;
	if(strncmp("DEL", command_dstr->cstring, 3) == 0)
		return DEL;

	if(strncmp("PUSH", command_dstr->cstring, 4) == 0)
		return PUSH;
	if(strncmp("POP", command_dstr->cstring, 3) == 0)
		return POP;

	if(strncmp("TRUNCATE", command_dstr->cstring, 6) == 0)
		return TRUNCATE;
	if(strncmp("APPEND", command_dstr->cstring, 6) == 0)
		return APPEND;

	if(strncmp("NEW", command_dstr->cstring, 3) == 0)
		return NEW;

	if(strncmp("RUN", command_dstr->cstring, 4) == 0)
		return RUN;

	if(strncmp("EXIT", command_dstr->cstring, 4) == 0)
		return EXIT;

	return ERROR;
}