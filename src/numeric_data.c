#include<numeric_data.h>

unsigned long long int get_location_of_decimal_point(dstring* data_p)
{
	unsigned long long int i = 0;
	while(data_p->cstring[i] != '\0' && data_p->cstring[i] != '.'){i++;}
	return i;
}

int is_negative(dstring* data_p)
{
	if(data_p->cstring[0] == '-')
	{
		return 1;
	}
	return 0;
}

void remove_leading_and_trailing_zeros(dstring* data_p)
{
	// todo
}

int compare_decimal(dstring* data_p1, dstring* data_p2)
{
	int p1_is_negative = is_negative(data_p1);
	int p2_is_negative = is_negative(data_p2);

	if(p1_is_negative != p2_is_negative)
	{
		return p2_is_negative ? 1 : -1;
	}

	remove_leading_and_trailing_zeros(data_p1);
	unsigned long long int data_p1_dec_loc = get_location_of_decimal_point(data_p1);

	remove_leading_and_trailing_zeros(data_p2);
	unsigned long long int data_p2_dec_loc = get_location_of_decimal_point(data_p2);

	if(data_p1_dec_loc == data_p2_dec_loc)
	{
		return compare_dstring(data_p1, data_p2);
	}
	else
	{
		return data_p1_dec_loc - data_p2_dec_loc;
	}
}