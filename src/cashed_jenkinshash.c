#include<cashed_jenkinshash.h>

#include<stdlib.h>
// The below function is one char at a time jenkins hash function
// source : https://en.wikipedia.org/wiki/Jenkins_hash_function
static unsigned int jenkins_one_at_a_time_hash(const char* key, unsigned int length) {
  unsigned int i = 0;
  unsigned hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

unsigned int modified_jenkins_hash_dstring(const dstring* key)
{
	return modified_jenkins_hash(get_byte_array_dstring(key), get_char_count_dstring(key));
}

unsigned int modified_jenkins_hash(const void* data_p, unsigned int size_in_bytes)
{
	return jenkins_one_at_a_time_hash(data_p, size_in_bytes);
}