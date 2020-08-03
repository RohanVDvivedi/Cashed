#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

#include<cashed_server.h>

#include<server.h>
#include<query.h>

#define QUERY_BUFFER_SIZE 1024

void connection_handler(int conn_fd, void* hashmap)
{
	// we do not accept commands greater than 1000 characters
	char buffer[QUERY_BUFFER_SIZE];
	dstring io_string;
	init_dstring(&io_string, "", 1002);

	// this is the query we build for every request
	query q = {};

	while(1)
	{
		// read data and write it to io_string
		int buffreadlength = recv(conn_fd, buffer, QUERY_BUFFER_SIZE-1, 0);
		if(buffreadlength == -1 || buffreadlength == 0)
			break;
		buffer[buffreadlength] = '\0';
		make_dstring_empty(&io_string);
		append_to_dstring(&io_string, buffer);

		// parse the io_string to buld the query object
		parse_query(&io_string, &q);
		
		// clear the io_string holding the query
		make_dstring_empty(&io_string);

		// process the query, and get result in the io_string
		process_query(&io_string, &q);
		
		// write response io_string to the client
		send(conn_fd, io_string.cstring, io_string.bytes_occupied - 1, 0);
		
		// clear the string
		make_dstring_empty(&io_string);
	}

	deinit_dstring(&io_string);
}

volatile int listen_fd = -1;

void intHandler(int dummy)
{
	printf("interrupt ctrl + c\n");
	if(listen_fd != -1)
	{
		printf("closing listenning socket\n");
    	server_stop(listen_fd);
    }
}

int start_cashed_server(int port)
{
	signal(SIGINT, intHandler);

	connection_group cgp = get_connection_group_tcp_ipv4("127.0.0.1", port);
	serve(&cgp, NULL, connection_handler, 10, &listen_fd);

	return 0;
}
