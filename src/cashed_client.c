#include<cashed_client.h>

#include<stdlib.h>

transaction_client* get_cashed_client(char* host, int port, int connection_count)
{
	connection_group cgp = get_connection_group_tcp_ipv4(host, port);
	return get_transaction_client(cgp, connection_count);
}

#define QUERY_BUFFER_SIZE 1024

int execute_cashed_query(int fd, c_query* query_p, c_result* result_p)
{
	if(query_p->cmd == ERR)
		return 0;

	int io_error = 0;
	dstring io_string;	init_dstring(&io_string, "", 0);

	serialize_query(&io_string, query_p);

	// send data that needs to be sent
	int buffsentlength = send(fd, io_string.cstring, io_string.bytes_occupied-1, 0);
	if(buffsentlength == -1 || buffsentlength == 0)
		io_error = 1;
	
	make_dstring_empty(&io_string);

	char buffer[QUERY_BUFFER_SIZE];

	if(!io_error)
	{
		// read the data back, as a response, untill a ';'' is received
		int semicolon_received = 0;
		while(!io_error && !semicolon_received)
		{
			int buffreadlength = recv(fd, buffer, QUERY_BUFFER_SIZE-1, 0);
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

		deserialize_result(&io_string, result_p);
	}

	deinit_dstring(&io_string);

	return io_error;
}

void* transact_cashed_query(int fd, int* close_connection_requested, void* query_p_v)
{
	c_result* result_p = calloc(1, sizeof(c_result));	init_result(result_p);
	c_query* query_p = query_p_v;
	int error = execute_cashed_query(fd, query_p, result_p);
	if(error == -1)
	{
		deinit_result(result_p);	free(result_p);

		*close_connection_requested = 1;
		return NULL;
	}
	return result_p;
}

void close_cashed_client(transaction_client* cashed_client)
{
	shutdown_transaction_client(cashed_client);
	delete_transaction_client(cashed_client);
}