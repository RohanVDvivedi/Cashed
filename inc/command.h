#ifndef COMMAND_H
#define COMMAND_H

typedef enum command command;
enum command
{
	GET,
	SET,
	NEW,
	DEL,
	EXEC
};

#endif