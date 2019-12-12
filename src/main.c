#include<main.h>

void basic_connection_handler(int conn_fd);

int main()
{
	// building the global_hash variable
	init_global_hash();

	// start listenning, for connections
	serve_tcp_on_ipv4(6969, basic_connection_handler);
	return 0;
}

void basic_connection_handler(int conn_fd)
{
	// we do not accept commands greater than 1000 characters
	char buffer[1000];
	dstring* sequence = get_dstring("", 1002);	

	// a new hashmap is built to store the connection session variables
	hashmap* connection_variables = get_connection_variables_hashmap();

	int close_connection = 0;

	while(!close_connection)
	{
		int buffreadlength = recv(conn_fd, buffer, 999, 0);
		if(buffreadlength == -1){break;}
		buffer[buffreadlength] = '\0';
		append_to_dstring(sequence, buffer);

		// this is the query we build for every request
		query* query = parse_query(sequence);

		// once the query is parsed, use the sequence to build the response to send
		make_dstring_empty(sequence);

		// if we have successfully parsed the query
		if(query != NULL)
		{
			Data* result = NULL;

			// process it
			int error = process_query(query, connection_variables, &result);

			if(error == 1)
			{
				close_connection = 1;
			}
			else if(error == 0)
			{
				append_to_dstring(sequence, "SUCCESS");

				// if processing the query is successfull and the 
				// result is generated, we need to serialize and print it
				if(result != NULL)
				{
					append_to_dstring(sequence, " : ");
					serialize_data(sequence, result);
				}
			}
			else if(error < 0)
			{
				append_to_dstring(sequence, "ERROR PROCESSING");
			}

			// delete the query once the execution is completed
			delete_query(query);
		}
		else
		{
			// else there was a error parsing the query
			append_to_dstring(sequence, "ERROR PARSING");
		}

		append_to_dstring(sequence, "\r\n");

		send(conn_fd, sequence->cstring, sequence->bytes_occupied - 1, 0);
		
		make_dstring_empty(sequence);
	}

	// connection variables are destroyed, as soon as the connection is destroyed
	delete_connection_variables_hashmap(connection_variables);
}
