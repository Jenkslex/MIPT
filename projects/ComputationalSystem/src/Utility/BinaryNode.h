#pragma once

template<typename T>
class BinaryNode
{
public:
	BinaryNode();
	~BinaryNode();

	BinaryNode*  Right();
	BinaryNode*  Left();

	void SetRight(BinaryNode* aChild);
	void SetLeft (BinaryNode* aChild);

	void RemoveRight();
	void RemoveLeft();

	T&   Data();
	void SetData(T aData);

private:
	BinaryNode<T>* left_;
	BinaryNode<T>* right_;

	T data_;
};