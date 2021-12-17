#include<stdio.h>
#include<signal.h>

#include<server.h>

#include<cashed_hashtable_def.h>
#include<cashed_process_query.h>

#define QUERY_BUFFER_EXP_SIZE 1024

void connection_handler(int conn_fd, void* cashtable_p_v)
{
	cashtable* cashtable_p = cashtable_p_v;

	int io_error = 0;

	dstring io_string;
	init_empty_dstring(&io_string, QUERY_BUFFER_EXP_SIZE);

	while(!io_error)
	{
		int semicolon_received = 0;
		while(!io_error && !semicolon_received)
		{
			if(get_capacity_dstring(&io_string) - get_char_count_dstring(&io_string) < QUERY_BUFFER_EXP_SIZE)
				expand_dstring(&io_string, get_char_count_dstring(&io_string) + QUERY_BUFFER_EXP_SIZE);

			// read data and write it to io_string
			int buffreadlength = recv(conn_fd, get_byte_array_dstring(&io_string) + get_char_count_dstring(&io_string), get_capacity_dstring(&io_string) - get_char_count_dstring(&io_string), 0);

			if(buffreadlength == -1 || buffreadlength == 0)
				io_error = 1;
			else
			{
				unsigned int iter = get_char_count_dstring(&io_string);

				// increment the bytes occupied on the io dstring
				io_string.bytes_occupied += buffreadlength;

				while(iter < get_char_count_dstring(&io_string) && !semicolon_received)
				{
					if(get_byte_array_dstring(&io_string)[iter++] == ';')
						semicolon_received = 1;
				}
			}
		}

		if(!io_error)
		{
			// init query and result
			c_query q;		init_query(&q, ERR);
			c_result r;		init_result(&r);

			// build the query from the what we read
			deserialize_query(&io_string, &q);

			// clear the io_string
			make_dstring_empty(&io_string);

			// process the query, and get result in the io_string
			process_query(cashtable_p, &q, &r);

			// parse the io_string to buld the query object
			serialize_result(&io_string, &r);

			// deinitialize query and result
			deinit_query(&q);
			deinit_result(&r);
			
			// write response io_string to the client
			int buffsentlength = send(conn_fd, get_byte_array_dstring(&io_string), get_char_count_dstring(&io_string), 0);
			if(buffsentlength == -1 || buffsentlength == 0)
				io_error = 1;
		}

		// clear the io_string
		make_dstring_empty(&io_string);
		shrink_dstring(&io_string);
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
