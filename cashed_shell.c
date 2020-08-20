#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<cashed_client.h>

int main(int argc, char** argv)
{
	unsigned int port = 6969;
	if(argc == 2)
		sscanf(argv[1], "%u", &port);

	transaction_client* cashed_client = get_cashed_client("127.0.0.1", port, 3);

	char query_buffer[1024];

	c_query q;
	dstring io_string;		init_dstring(&io_string, "", 0);

	while(1)
	{
		printf("client ->");
		scanf("%s", query_buffer);

		if(strcmp(query_buffer, "exit") == 0)
			break;

		make_dstring_empty(&io_string);
		append_to_dstring(&io_string, query_buffer);

		init_query(&q, ERR);
		deserialize_query(&io_string, &q);

		//print_query(&q);

		job* query_promise = queue_transaction(cashed_client, transact_cashed_query, &q);

		if(query_promise != NULL)
		{
			c_result* r_p = get_result_for_transaction(query_promise, NULL);
			printf("server ->");
			print_result(r_p);
			deinit_result(r_p);
			free(r_p);
		}

		deinit_query(&q);
	}

	deinit_dstring(&io_string);

	close_cashed_client(cashed_client);
	return 0;
}