#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

#include<server.h>

#include<query.h>
#include<result.h>

#include<cashed_hashtable.h>

#define QUERY_BUFFER_SIZE 1024

void connection_handler(int conn_fd, void* cashtable_p_v)
{
	cashtable* cashtable_p = cashtable_p_v;

	// we do not accept commands greater than 1000 characters
	char buffer[QUERY_BUFFER_SIZE];

	int io_error = 0;
	dstring io_string;
	init_dstring(&io_string, "", 0);

	// this is the query we build for every request, and its result that we will send
	query q = {};
	result r = {};

	while(!io_error)
	{
		// clear the io_string holding the query
		make_dstring_empty(&io_string);

		int semicolon_received = 0;
		while(!io_error && !semicolon_received)
		{
			// read data and write it to io_string
			int buffreadlength = recv(conn_fd, buffer, QUERY_BUFFER_SIZE-1, 0);

			if(buffreadlength == -1 || buffreadlength == 0)
				io_error = 1;
			else
			{
				buffer[buffreadlength] = '\0';
				int iter = 0;
				while(iter < buffreadlength && !semicolon_received)
				{
					if(buffer[iter++] == ';')
						semicolon_received = 1;
				}
				append_to_dstring(&io_string, buffer);
			}
		}

		if(!io_error)
		{
			// initialize query and result
			init_query(&q);
			init_result(&r);

			// build the query from the data that we read
			deserialize_query(&io_string, &q);

			// process the query, and get result in the io_string
			process_query(cashtable_p, &q, &r);

			// clear the io_string holding the query
			make_dstring_empty(&io_string);
			// parse the io_string to buld the query object
			serialize_result(&io_string, &r);

			// deinitialize query and result
			deinit_query(&q);
			deinit_result(&r);
			
			// write response io_string to the client
			int buffsentlength = send(conn_fd, io_string.cstring, io_string.bytes_occupied-1, 0);
			if(buffsentlength == -1 || buffsentlength == 0)
				io_error = 1;
		}
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

void start_cashed_server(int port)
{
	signal(SIGINT, intHandler);

	connection_group cgp = get_connection_group_tcp_ipv4("127.0.0.1", port);

	cashtable cashed_server_cashtable;

	init_cashtable(&cashed_server_cashtable, 50);

	serve(&cgp, &cashed_server_cashtable, connection_handler, 10, &listen_fd);

	deinit_cashtable(&cashed_server_cashtable);
}
