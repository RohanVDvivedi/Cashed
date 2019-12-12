#ifndef TYPE_OF_DATA_H
#define TYPE_OF_DATA_H

#include<dstring.h>

// every data in the RoareDB can be of one of these types

typedef enum TypeOfData TypeOfData;
enum TypeOfData
{
	// simple data structures

	// array
	ARRAY,

	// stack
	STACK,

	// queue
	QUEUE,

	// linkedlist
	LINKED_LIST,

	// complex data structures, that store key value pair

	// balancedbst
	BINARY_SEARCH_TREE,

	// hashmap
	HASHTABLE,

	// heap
	HEAP,

	// Datatypes supported

	// dstring from cutlery
	STRING,

	// long long int
	NUM_INTEGER,

	// double
	NUM_FLOAT,

	// a number stored as string
	NUM_DECIMAL,

	// unsigned long long int, storing the nanoseconds since epoch
	TIME_STAMP,

	// this is of type unsigned character, it maps to a string stored in system tables
	ENUMERATION,

	// this is address to another data, it can be in any other node, and any where
	// yet to be defined properly
	POINTER,

	// code that can be executed
	PROGRAM,

	// unidentified datatype to show error in parsing the query
	UNIDENTIFIED
};

// converts a dstring containg the TypeOfData as string to its enum type
TypeOfData get_type_from_dstring(dstring* type);

// converts TypeOfData enum type to its name as dstring and appends it to the end of type_dstring
void serialize_type(dstring* type_dstring, TypeOfData type);

#endif