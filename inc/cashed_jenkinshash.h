#ifndef JENKINS_HASH_H
#define JENKINS_HASH_H

#include<stdint.h>
#include<dstring.h>

unsigned int modified_jenkins_hash_dstring(const dstring* key);

unsigned int modified_jenkins_hash(const void* data_p, unsigned int size_in_bytes);

#endif