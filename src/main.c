#include<stdio.h>
#include<stdlib.h>

#include<server.h>
#include<hashmap.h>

void connection_handler(int conn_fd);

int main()
{
	hashTable = get_hashmap();

	serve_tcp_on_ipv4(6969, connection_handler);

	return 0;
}

void connection_handler(int conn_fd)
{
	// we do not accept commands greater than 1000 characters
	char buffer[1000];

	while(1)
	{
		int buffreadlength = recv(conn_fd, buffer, 999, 0);
		if(buffreadlength == -1)
		{
			break;
		}

		buffer[buffreadlength] = '\0';

		// process the buffer here
		if(process(buffer) != 0)
		{
			break;
		}

		buffreadlength = strlen(buffer);
		send(conn_fd, buffer, buffreadlength, 0);
	}
}
