#ifndef COMMAND_DEF_H
#define COMMAND_DEF_H

typedef enum command command;
enum command
{
	ERR = 0,
	GET,
	SET,
	DEL
};

#define SUPPORTED_COMMANDS_COUNT 4

#endif