#include<cashed_command.h>

#include<string.h>

int supported_commands_count = SUPPORTED_COMMANDS_COUNT;

char* command_strings[SUPPORTED_COMMANDS_COUNT] = {
	"err",
	"get",
	"set",
	"del"
};

command deserialize_command(dstring* command_dstr)
{
	int iter = 1;
	while(iter < SUPPORTED_COMMANDS_COUNT)
	{
		if(strncmp(command_strings[iter], command_dstr->cstring, strlen(command_strings[iter])) == 0)
			return ((command)iter);
		iter++;
	}

	return ERR;
}

void serialize_command(command cmd, dstring* append_to_dstr)
{
	append_to_dstring(append_to_dstr, command_strings[cmd]);
}