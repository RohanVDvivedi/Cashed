#ifndef PARSE_QUERY_H
#define PARSE_QUERY_H

#include<query.h>

// returns NULL, if there is parsing error for a wrong syntax query
// else will return a query
query* parse_query(dstring* requestSequence);

#endif