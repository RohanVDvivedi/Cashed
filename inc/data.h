#ifndef DATA_H
#define DATA_H

#include<time.h>

#include<hashmap.h>

#include<rwlock.h>

#include<typeofdata.h>

#include<enum_data.h>
#include<numeric_data.h>


#define TIME_STAMP_FORMAT "%Y:%m:%dT%H:%M:%S"
#define SCAN_TIME_STAMP(str, tim) sscanf(str, "%d:%d:%dT%d:%d:%d", &(tim.tm_year), &(tim.tm_mon), &(tim.tm_mday) , &(tim.tm_hour) , &(tim.tm_min) , &(tim.tm_sec)); tim.tm_year -= 1900; tim.tm_mon--;

typedef struct Data Data;
struct Data
{
	// the type of the data, it stores
	TypeOfData type;

	// lock (reader writer lock)
	rwlock* rwL;

	// the pointer to the data
	void* value;
};

Data* get_new_data(TypeOfData type, dstring* input_param);

void transfer_data(Data* destination,Data* source);

int compare_data(const Data* data_p1, const Data* data_p2);

unsigned long long int hash_data(const Data* data_p);

void serialize_data(dstring* destination, const Data* data_p);

// deletes value of the data, not the lock or the type and all other stuff
void delete_data_contents(Data* data_p);

void delete_data(Data* data_p);

#endif