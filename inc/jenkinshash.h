#ifndef JENKINS_HASH_H
#define JENKINS_HASH_H

#include<stdint.h>
#include<dstring.h>

unsigned long long int jenkins_hash(const void* data_p, unsigned long long int size_in_bytes);

#endif