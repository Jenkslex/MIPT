#include <iostream>
#include <string>

#include "HashTable.h"

int main() {
	HashTable<std::string> table(1000);

	table.insert(std::string("Anna"));
	table.insert(std::string("Gleb"));

	for (auto elem : table.getElements())
		std::cout << elem << std::endl;
	
	system("pause");
	return 0;
}