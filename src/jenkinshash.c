#include<jenkinshash.h>

unsigned long long int jenkins_hash(const dstring* string_p)
{
	unsigned long long int result = ((string_p->bytes_occupied << 3) % 397);
	result = result | (result << 10);
	result = (((result + string_p->bytes_occupied) * 8353) % 8513) + string_p->bytes_occupied;
	result = result * string_p->cstring[0];
	result = result * string_p->cstring[string_p->bytes_occupied - 2];
	result = result * 9973;
	return result;
}