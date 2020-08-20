#ifndef COMMAND_H
#define COMMAND_H

#include<dstring.h>

typedef enum c_command c_command;
enum c_command
{
	ERR = 0,
	GET,
	SET,
	DEL
};

#define SUPPORTED_COMMANDS_COUNT 4

extern char* c_command_strings[];

// compares the first few characters from dstring to identify the command
c_command deserialize_command(dstring* command_dstr);

void serialize_command(c_command cmd, dstring* append_to_dstr);

#endif