#include<cashed_client.h>

transaction_client* get_cashed_client(char* host, int port, int connection_count)
{
	connection_group cgp = get_connection_group_tcp_ipv4(host, port);
	return get_transaction_client(cgp, 3);
}

#define QUERY_BUFFER_SIZE 1024

int execute_query(int fd, query* query_p, result* result_p)
{
	if(query_p->cmd == ERR)
		return 0;

	dstring io_string;	init_dstring(&io_string, "", 0);

	serialize_command(query_p->cmd, &io_string);
	append_to_dstring(&io_string, "(");
	append_to_dstring(&io_string, query_p->key.cstring);
	if(query_p->cmd == SET)
	{
		append_to_dstring(&io_string, ",");
		append_to_dstring(&io_string, query_p->value.cstring);
	}
	append_to_dstring(&io_string, ");");

	// send data that needs to be sent
	int buffsentlength = send(fd, io_string.cstring, io_string.bytes_occupied-1, 0);
	deinit_dstring(&io_string);
	if(buffsentlength == -1 || buffsentlength == 0)
		return -1;

	// read the data back, as a response, untill a ; is received
	char buffer[QUERY_BUFFER_SIZE];
	int error = 0; int semicolon_received = 0;
	int response_packet = 0;
	while(!error && !semicolon_received)
	{
		int buffreadlength = recv(fd, buffer, QUERY_BUFFER_SIZE-1, 0);
		if(buffreadlength == -1 || buffreadlength == 0)
				error = 1;
		else
		{
			buffer[buffreadlength] = '\0';
			int iter = 0;
			while(iter < buffreadlength && !semicolon_received)
			{
				if(buffer[iter++] == ';')
					semicolon_received = 1;
			}

			int buffer_copy_offset = 0;
			if(response_packet == 0)
			{
				if(buffer[0] == '1')
					result_p->success = 1;
				buffer_copy_offset++;
			}
			append_to_dstring(&(result_p->data), buffer + buffer_copy_offset);
		}
		response_packet++;
	}

	return 0;
}

result* transact_query(int fd, int* close_connection_requested, query* query_p)
{
	result* result_p = calloc(1, sizeof(result));
	init_dstring(&(result_p->data), "", 0);
	int error = execute_query(fd, query_p, result_p);
	if(error == -1)
	{
		deinit_dstring(&(result_p->data));
		free(result_p);
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