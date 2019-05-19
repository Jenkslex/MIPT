#pragma once

#include <vector>
#include <stack>

#include "Tables.h"

class CPU
{
public:
	CPU();
	~CPU();

	void Execute(std::vector<int>& program);

	//void SetOutStream(std::ostream& outStream);
	//void SetInStream(std::istream& inStream);

	void Dump(); // Temporary

private:
	template<CPU_Command command> void ExecuteCommand(std::vector<int>& program);

	int GetArgument(std::vector<int>& program);
	void Push(int number);
	int  Pop();

	template<template<class>class PRED> void ArithmeticalOperation();
	template<template<class>class PRED> void ConditionalJump(std::vector<int>& program);
	template<template<class>class PRED> void Condition();

	//std::ostream outStream_;
	//std::istream inStream_;

	bool exitFlag_;
	int instructionPointer_;
	int basePointer_;

	std::vector<int> stack_;
	std::vector<int> registers_;
};

