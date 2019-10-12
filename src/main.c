#include<stdio.h>
#include<stdlib.h>

#include<main.h>

void basic_connection_handler(int conn_fd);

int main()
{
	hashTable = get_hashmap(HASHSLOTS, (unsigned long long int (*)(const void*))jenkins_hash, (int (*)(const void*, const void*))compare_dstring, ELEMENTS_AS_RED_BLACK_BST);
	serve_tcp_on_ipv4(6969, basic_connection_handler);
	return 0;
}

hashmap* hashTable;

void basic_connection_handler(int conn_fd)
{
	// we do not accept commands greater than 1000 characters
	char buffer[1000];
	dstring* sequence = get_dstring("", 1002);

	// this is the query we build for every request
	query q;

	while(1)
	{
		int buffreadlength = recv(conn_fd, buffer, 999, 0);
		if(buffreadlength == -1){break;}
		buffer[buffreadlength] = '\0';
		sequence = append_to_dstring(sequence, buffer);

		if(parse_statefull_request(sequence, &q) == 1)
		{
			if(process_query(sequence, &q))
			{
				break;
			}
			send(conn_fd, sequence->cstring, sequence->bytes_occupied - 1, 0);
			make_dstring_empty(sequence);
		}
	}
}
