#include "BinaryNode.h"


template<typename T>
BinaryNode<T>::BinaryNode():
	left_ (nullptr),
	right_(nullptr)
{
}

template<typename T>
BinaryNode<T>::~BinaryNode()
{
	RemoveLeft();
	RemoveRight();
}

template<typename T>
BinaryNode<T>* BinaryNode<T>::Right()
{
	return right_;
}

template<typename T>
BinaryNode<T>* BinaryNode<T>::Left()
{
	return left_;
}

template<typename T>
void BinaryNode<T>::SetRight(BinaryNode * aChild)
{
	right_ = aChild;
}

template<typename T>
void BinaryNode<T>::SetLeft(BinaryNode * aChild)
{
	left_ = aChild;
}

template<typename T>
void BinaryNode<T>::RemoveRight()
{
	if (right_ != nullptr)
	{
		delete right_;
		right_ = nullptr;
	}
}

template<typename T>
void BinaryNode<T>::RemoveLeft()
{
	if (left_ != nullptr)
	{
		delete left_;
		left = nullptr;
	}
}

template<typename T>
T& BinaryNode<T>::Data()
{
	return data_;
}

template<typename T>
void BinaryNode<T>::SetData(T aData)
{
	data_ = aData;
}
