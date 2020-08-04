#ifndef COMMAND_H
#define COMMAND_H

#include<dstring.h>

typedef enum command command;
enum command
{
	ERR = 0,
	GET,
	SET,
	DEL,
	EXISTS,
	PUSH,
	POP
};

extern char* command_strings[];

// compares the first few characters from dstring to identify the command
command deserialize_command(dstring* command_dstr);

void serialize_command(command cmd, dstring* append_to_dstr);

#endif