#include<command.h>

command deserialize_command(dstring* command_dstr)
{
	if(strncmp("get", command_dstr->cstring, 3) == 0)
		return GET;
	if(strncmp("set", command_dstr->cstring, 3) == 0)
		return SET;
	if(strncmp("del", command_dstr->cstring, 3) == 0)
		return DEL;

	return ERR;
}

void serialize_command(command cmd, dstring* append_to_dstr)
{
	switch(cmd)
	{
		case GET :
			append_to_dstring(append_to_dstr, "get");
			return;
		case SET :
			append_to_dstring(append_to_dstr, "set");
			return;
		case ERR :
			append_to_dstring(append_to_dstr, "del");
			return;
	}
}