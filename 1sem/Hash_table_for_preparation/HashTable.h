#pragma once

#include <vector>
#include <list>


template <typename T>
class HashTable {
public:
	HashTable(int size);
	~HashTable();

	void insert(T elem);
	bool isInTable(T elem);

	std::list<T> getElements();

private:
	std::list<T>& getListByElement(T elem);
	
	bool isInList(std::list<T>& list, T elem);
	unsigned int hash(T elem);

	std::vector<std::list<T>> array_;
	int size_;
};

template <typename T>
HashTable<T>::HashTable(int size) :
	size_(size)
{
	array_.resize(size);
}

template <typename T>
HashTable<T>::~HashTable()
{
}

template <typename T>
void HashTable<T>::insert(T elem)
{
	auto& list = getListByElement(elem);

	if (isInList(list, elem))
		return;

	list.push_front(elem);
}

template <typename T>
bool HashTable<T>::isInTable(T elem)
{
	auto& list = getListByElement(elem);

	if (isInList(list, elem))
		return true;

	return false;
}

template <typename T>
std::list<T> HashTable<T>::getElements()
{
	std::list<T> outputList;

	for (auto list : array_)
		for (auto elem : list)
			outputList.push_front(elem);

	return outputList;
}

template <typename T>
std::list<T>& HashTable<T>::getListByElement(T elem)
{
	unsigned int result = hash(elem) % size_;
	return array_[result];
}

template <typename T>
bool HashTable<T>::isInList(std::list<T>& list, T string)
{
	auto it = std::find(list.begin(), list.end(), string);

	if (it != list.end())
		return true;

	return false;
}

template <typename T>
unsigned int HashTable<T>::hash(T elem)
{
	unsigned int size = sizeof(elem);
	char* data = (char*) &elem;

	unsigned int h = 0;

	for (unsigned int i = 0; i < size; i++)
		h = (h << 1) ^ (unsigned int) data[i];

	return h;
}

template <>
unsigned int HashTable<std::string>::hash(std::string elem);
