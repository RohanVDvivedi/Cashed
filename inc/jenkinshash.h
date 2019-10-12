#ifndef JENKINS_HASH_H
#define JENKINS_HASH_H

#include<dstring.h>

unsigned long long int jenkins_hash(const dstring* string_p);

#endif