#include<cashed_command.h>

#include<string.h>

int supported_commands_count = SUPPORTED_COMMANDS_COUNT;

char* c_command_strings[SUPPORTED_COMMANDS_COUNT] = {
	"err",
	"get",
	"set",
	"del"
};

c_command deserialize_command(dstring* command_dstr)
{
	int iter = 1;
	while(iter < SUPPORTED_COMMANDS_COUNT)
	{
		if(compare_dstring_cstring(command_dstr, c_command_strings[iter]) == 0)
			return ((c_command)iter);
		iter++;
	}

	return ERR;
}

void serialize_command(c_command cmd, dstring* append_to_dstr)
{
	append_to_dstring(append_to_dstr, c_command_strings[cmd]);
}