#ifndef COMMAND_H
#define COMMAND_H

#include<dstring.h>

typedef enum command command;
enum command
{
	// for internal hashmaps, balanced bsts, bucketed linkedlist
	GET,	// insert
	SET,	// update insert
	DEL,	// delete

	// for internal queues, stacks and heaps
	PUSH,
	POP,

	// for string
	TRUNCATE,	// truncate the string to a particular length, truncates string to all of the length if not length is provided
	APPEND,		// append a static string or string from another variable to the given string

	// crerate a data structure type for your purpose
	NEW,

	// this command can execute a PROGRAM datatye, and a result is returned to you
	RUN,

	// to exit the connection
	EXIT,

	// if the command matches none of the above
	ERROR = -1
};

// compares the first few characters from dstring to identify the command
command identify_command(dstring* command_dstr);

#endif