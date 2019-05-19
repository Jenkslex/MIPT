#include "HashTable.h"

template <>
unsigned int HashTable<std::string>::hash(std::string elem)
{
	unsigned int h = 0;

	for (auto c : elem)
		h = (h << 1) ^ (unsigned int) c;

	return h;
}