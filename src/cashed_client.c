#include<cashed_client.h>

transaction_client* get_cashed_client(char* host, int port, int connection_count)
{
	connection_group cgp = get_connection_group_tcp_ipv4(host, port);
	return get_transaction_client(cgp, 3);
}

void* execute_query(int fd, int* close_connection_requested, void* additional_params);

dstring* GET_cashed(transaction_client* cashed_client, dstring* key)
{
	job* job_0 = queue_transaction(cashed_client, execute_query, NULL);
}

int SET_cashed(transaction_client* cashed_client, dstring* key, dstring* value)
{
	job* job_0 = queue_transaction(cashed_client, execute_query, NULL);
}

int DEL_cashed(transaction_client* cashed_client, dstring* key)
{
	job* job_0 = queue_transaction(cashed_client, execute_query, NULL);
}

void close_cashed_client(transaction_client* cashed_client)
{
	shutdown_transaction_client(cashed_client);
	delete_transaction_client(cashed_client);
}

void* execute_query(int fd, int* close_connection_requested, void* additional_params)
{
	dstring* data_to_send = (dstring*) additional_params;

	// send data that needs to be sent
	int buffsentlength = send(fd, data_to_send->cstring, data_to_send->bytes_occupied, 0);
	if(buffsentlength == -1 || buffsentlength == 0)
	{
		(*close_connection_requested) = 1;
		return NULL;
	}

	// read the data back, as a response
	char buffer[1000];
	int buffreadlength = recv(fd, buffer, 999, 0);
	if(buffreadlength == -1 || buffreadlength == 0)
	{
		(*close_connection_requested) = 1;
		return NULL;
	}
	buffer[buffreadlength] = '\0';

	return get_dstring(buffer, 10);
}