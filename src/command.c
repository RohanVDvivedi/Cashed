#include<command.h>

command identify_command(dstring* command_dstr)
{
	if(strncmp("GET", command_dstr->cstring, 3) == 0)
		return GET;
	if(strncmp("SET", command_dstr->cstring, 3) == 0)
		return SET;
	if(strncmp("DEL", command_dstr->cstring, 3) == 0)
		return DEL;

	return ERR;
}