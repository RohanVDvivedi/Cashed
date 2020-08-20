#ifndef COMMAND_H
#define COMMAND_H

#include<cashed_command_def.h>
#include<dstring.h>

extern char* command_strings[];

// compares the first few characters from dstring to identify the command
command deserialize_command(dstring* command_dstr);

void serialize_command(command cmd, dstring* append_to_dstr);

#endif