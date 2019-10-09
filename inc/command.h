#ifndef COMMAND_H
#define COMMAND_H

typedef enum command command;
enum command
{
	// for the main hash table and intenal hashes, bsts
	GET,
	SET,

	// for internal queues, stacks and heaps
	PUSH,
	POP,

	// crerate a a new data structure type for your purpose
	NEW,

	// delete the data structure and all its internals
	DEL,

	// this command can execute a PRAGRAM datatye, and a result is returned to you
	EXEC
};

#endif