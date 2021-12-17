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
		if(is_prefix(command_dstr, &get_literal_cstring(c_command_strings[iter])))
			return ((c_command)iter);
		iter++;
	}

	return ERR;
}

void serialize_command(c_command cmd, dstring* append_to_dstr)
{
	concatenate_dstring(append_to_dstr, &get_literal_cstring(c_command_strings[cmd]));
}