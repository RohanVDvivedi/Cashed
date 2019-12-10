#ifndef COMMAND_H
#define COMMAND_H

#include<dstring.h>

typedef enum command command;
enum command
{
	// for the main hash table and intenal hashes, bsts
	GET, 				// (data_structure, key) -> value
	SET, 				// (data_structure, key, value) -> Null
	DEL, 				// (data_structure, key) -> 1 or 0 (number of elements deleted)

	// for internal queues, stacks and heaps
	PUSH,				// (data_structure) -> Null
	POP,				// (data_structure) -> Null
	TOP, 				// (data_structure) -> value (the element that will be removed, if POP was called)

	// for internal hashmaps, balanced bsts, bucketed linkedlist
	INSERT,
	SEARCH,
	UPDATE,
	DELETE,

	// for numbers
	INCREMENT_BY_1,
	DECREMENT_BY_1,

	// for string
	APPEND,

	// crerate a data structure type for your purpose
	NEW,

	// this command can execute a PROGRAM datatye, and a result is returned to you
	EXEC,

	// to exit the connection
	EXIT,

	// if the command matches none of the above
	ERROR = -1
};

// compares the first few characters from dstring to identify the command
command identify_command(dstring* command_dstr);

#endif