#include<query.h>

query* parse_query(dstring* requestSequence)
{
	// todo
	return NULL;
}

void delete_query(query* query_p)
{
	// todo
}

int process_query(dstring* responseSequence, query* query_p, hashmap* connection_variables)
{
	make_dstring_empty(responseSequence);

	int exit_called = 0;
// todo but similar to previous logic
/*
	switch(query_p->command)
	{
		case GET :
		{
			Data* key = get_new_data(query_p->key);
			Data* value = (Data*)(find_value_from_hash(hashTable, key));
			delete_data(key);
			serialize_data(responseSequence, value);
			break;
		}
		case SET :
		{
			Data* key = get_new_data(query_p->key);
			Data* value = (Data*)(find_value_from_hash(hashTable, key));
			if(value == NULL)
			{
				value = get_new_data(query_p->value);
				if(value->type != UNIDENTIFIED)
				{
					insert_entry_in_hash(hashTable, key, value);
					append_to_dstring(responseSequence, "INSERTED");
				}
				else
				{
					append_to_dstring(responseSequence, "ERROR");
					delete_data(value);
				}
			}
			else
			{
				Data* value_new = get_new_data(query_p->value);
				if(value_new->type != UNIDENTIFIED)
				{
					transfer_data(value, value_new);
					append_to_dstring(responseSequence, "UPDATED");
				}
				else
				{
					append_to_dstring(responseSequence, "ERROR");
				}
				delete_data(value_new);
				delete_data(key);
			}
			break;
		}
		case DEL :
		{
			Data* key = get_new_data(query_p->key);
			Data* return_key;
			Data* return_value;
			int elements_deleted = delete_entry_from_hash(hashTable, key, (const void**)(&return_key), (const void**)(&return_value));
			if(elements_deleted == 1)
			{
				delete_data(return_key);
				delete_data(return_value);
				append_to_dstring(responseSequence, "DELETED");
			}
			else
			{
				append_to_dstring(responseSequence, "NO SUCH ELEMENTS");
			}
			delete_data(key);
			break;
		}
		case EXIT :
		default :
		{
			exit_called = 1;
			break;
		}
	}
*/
	return exit_called;
}