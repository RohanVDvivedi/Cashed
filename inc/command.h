#ifndef COMMAND_H
#define COMMAND_H

#include<dstring.h>

typedef enum command command;
enum command
{
	GET,	// insert
	SET,	// update insert
	DEL,	// delete

	ERR = -1
};

// compares the first few characters from dstring to identify the command
command deserialize_command(dstring* command_dstr);

void serialize_command(command cmd, dstring* append_to_dstr);

#endif