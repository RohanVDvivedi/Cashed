#ifndef COMMAND_H
#define COMMAND_H

#include<data.h>
#include<dstring.h>

typedef enum command command;
enum command
{
	// for hashmaps, balanced bsts, bucketed linkedlist, array
	GET, 				// (data_structure, key) -> value
	SET, 				// (data_structure, key, value) -> 1 or 0 (0 if a new value is inserted or no operation, 1 if the old value is updated with data from new value)
	DEL, 				// (data_structure, key, return_key, return_value) -> 1 or 0 (number of elements deleted)

	// for internal queues, stacks and heaps
	PUSH,				// (data_structure, value) -> Null
	POP,				// (data_structure, return_value) -> Null
	TOP, 				// (data_structure) -> value (the element that will be removed, if POP was called)

	// for hashmaps, balanced bsts, bucketed linkedlist, array
	SEARCH,				// (data_structure, key) -> value
	INSERT,				// (data_structure, key, value) -> Null
	UPDATE,				// (data_structure, key, value) -> 1 or 0 (number of elements updated)
	DELETE,				// (data_structure, key, return_key, return_value) -> 1 or 0 (number of elements deleted)

	// return_key, return_value are the variables that are returned, when a referenece if no longer held for that data, after execution of the command

	// for node of a linkedlist
	NEXT,
	PREVIOUS,

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

// this are the implementations for the supported functions

Data* GET_command(Data* data_structure, Data* key);
int SET_command(Data* data_structure, Data* key, Data* value);
int DEL_command(Data* data_structure, Data* key, Data** return_key, Data** return_value);

Data* SEARCH_command(Data* data_structure, Data* key);
void INSERT_command(Data* data_structure, Data* key, Data* value);
int UPDATE_command(Data* data_structure, Data* key, Data* value);
int DELETE_command(Data* data_structure, Data* key, Data** return_key, Data** return_value);

void PUSH_command(Data* data_structure);
void POP_command(Data* data_structure);
Data* TOP_command(Data* data_structure);

#endif
