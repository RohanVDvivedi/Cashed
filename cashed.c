#include<stdio.h>
#include<stdlib.h>

#include<cashed_server.h>

int main(int argc, char** argv)
{
	unsigned int port = 6969;
	if(argc == 2)
		sscanf(argv[1], "%u", &port);
	start_cashed_server(port);
	return 0;
}