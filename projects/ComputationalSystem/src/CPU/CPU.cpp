#include "CPU.h"

#include <functional>
#include <iostream>
#include <string> 

//===================================================================================
CPU::CPU()
{
	registers_ = {0, 0, 0, 0};

	bool exitFlag_ = false;

	//SetInStream(std::cin);
	//SetOutStream(std::cout);
}
//!----------------------------------------------------------------------------------
CPU::~CPU()
{
}
//!----------------------------------------------------------------------------------
/*
void CPU::SetOutStream(std::ostream& outStream)
{
	outStream_ = outStream;
}
//!----------------------------------------------------------------------------------
void CPU::SetInStream(std::istream& inStream)
{
	inStream_ = inStream;
}*/
//===================================================================================


//===================================================================================
void CPU::Dump()
{
	std::cout << "========= CPU DUMP ===========" << std::endl;

	std::cout << "Stack: ";
	for (auto unit : stack_)
		std::cout << unit << " ";
	std::cout << std::endl;

	std::cout << "Registers: ";
	for (auto unit : registers_)
		std::cout << unit << " ";
	std::cout << std::endl;

	std::cout << "==============================" << std::endl;
}
//===================================================================================

//===================================================================================
int CPU::GetArgument(std::vector<int>& program)
{
	int argument = program[instructionPointer_];
	instructionPointer_++;

	return argument;
}
//!----------------------------------------------------------------------------------
void CPU::Push(int number)
{
	stack_.push_back(number);
}
//!----------------------------------------------------------------------------------
int CPU::Pop()
{
	if (stack_.empty())
		throw std::runtime_error("Stack is empty on IP " + 
			std::to_string(instructionPointer_));

	int number = stack_.back();
	stack_.pop_back();

	return number;
}
//===================================================================================

//===================================================================================
template<>
void CPU::ExecuteCommand<CMD_EXIT>(std::vector<int>& program)
{
	exitFlag_ = true;
}
//===================================================================================


//===================================================================================
template<>
void CPU::ExecuteCommand<CMD_PUSHCONST>(std::vector<int>& program)
{
	int number = GetArgument(program);
	Push(number);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_PUSHREG>(std::vector<int>& program)
{
	int registerNumber = GetArgument(program);
	int number = registers_[registerNumber];
	Push(number);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_PUSHLOC>(std::vector<int>& program)
{
	int shift = GetArgument(program);
	int value = stack_[basePointer_ + shift];
	Push(value);
}
//===================================================================================

//===================================================================================
template<>
void CPU::ExecuteCommand<CMD_LOADREG>(std::vector<int>& program)
{
	int registerNumber = GetArgument(program);
	int number = Pop();
	registers_[registerNumber] = number;
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_LOADLOC>(std::vector<int>& program)
{
	int shift = GetArgument(program);
	int newValue = Pop();
	stack_[basePointer_ + shift] = newValue;
}
//===================================================================================

//===================================================================================
template<template<class>class PRED>
void CPU::ArithmeticalOperation() 
{
	int b = Pop();
	int a = Pop();

	int result = PRED<int>()(a, b);

	Push(result);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_ADD>(std::vector<int>& program)
{
	ArithmeticalOperation<std::plus>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_SUB>(std::vector<int>& program)
{
	ArithmeticalOperation<std::minus>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_MUL>(std::vector<int>& program)
{
	ArithmeticalOperation<std::multiplies>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_DIV>(std::vector<int>& program)
{
	ArithmeticalOperation<std::divides>();
}
//===================================================================================


//===================================================================================
template<>
void CPU::ExecuteCommand<CMD_JMP>(std::vector<int>& program)
{
	int destination = GetArgument(program);
	instructionPointer_ = destination;
}
//!----------------------------------------------------------------------------------
template<template<class>class PRED>
void CPU::ConditionalJump(std::vector<int>& program)
{
	int a = Pop();
	int b = Pop();

	bool result = PRED<int>()(a, b);

	int destination = GetArgument(program);

	if (result)
		instructionPointer_ = destination;
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_JA>(std::vector<int>& program)
{
	ConditionalJump<std::greater>(program);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_JAE>(std::vector<int>& program)
{
	ConditionalJump<std::greater_equal>(program);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_JB>(std::vector<int>& program)
{
	ConditionalJump<std::less>(program);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_JBE>(std::vector<int>& program)
{
	ConditionalJump<std::less_equal>(program);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_JE>(std::vector<int>& program)
{
	ConditionalJump<std::equal_to>(program);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_JNE>(std::vector<int>& program)
{
	ConditionalJump<std::not_equal_to>(program);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_JNT>(std::vector<int>& program)
{
	int a = Pop();

	bool result = true;
	if (a != 0)
		result = false;

	int destination = GetArgument(program);

	if (result)
		instructionPointer_ = destination;
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_CALL>(std::vector<int>& program)
{
	int destination = GetArgument(program);
	Push(instructionPointer_);
	instructionPointer_ = destination;
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_RET>(std::vector<int>& program)
{
	if (stack_.empty()) 
	{
		exitFlag_ = true;
		return;
	}

	int destination = Pop();

	instructionPointer_ = destination;
}
//===================================================================================


//===================================================================================
template<template<class>class PRED>
void CPU::Condition()
{
	int b = Pop();
	int a = Pop();

	bool result = PRED<int>()(a, b);

	Push(result);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_EQUAL>(std::vector<int>& program)
{
	Condition<std::equal_to>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_NOTEQ>(std::vector<int>& program)
{
	Condition<std::not_equal_to>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_BOREQ>(std::vector<int>& program)
{
	Condition<std::less_equal>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_AOREQ>(std::vector<int>& program)
{
	Condition<std::greater_equal>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_BELOW>(std::vector<int>& program)
{
	Condition<std::less>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_ABOVE>(std::vector<int>& program)
{
	Condition<std::greater>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_AND>(std::vector<int>& program)
{
	Condition<std::bit_and>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_OR>(std::vector<int>& program)
{
	Condition<std::bit_or>();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_NEGATE>(std::vector<int>& program)
{
	int a = Pop();
	bool result = true;

	if (a != 0)
		result = false;

	Push(result);
}
//===================================================================================


//===================================================================================
template<>
void CPU::ExecuteCommand<CMD_NEW>(std::vector<int>& program)
{
	Push(0);
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_CRT_FRAME>(std::vector<int>& program)
{
	Push(basePointer_);
	basePointer_ = stack_.size() - 1;
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_DLT_FRAME>(std::vector<int>& program)
{
	while (basePointer_ != stack_.size() - 1)
		Pop();

	basePointer_ = Pop();
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_IN_LOC>(std::vector<int>& program)
{
	int newValue = 0;
	std::cin >> newValue;
	int shift = GetArgument(program);
	stack_[basePointer_ + shift] = newValue;
}
//!----------------------------------------------------------------------------------
template<>
void CPU::ExecuteCommand<CMD_OUT_LOC>(std::vector<int>& program)
{
	int shift = GetArgument(program);
	int value = stack_[basePointer_ + shift];
	std::cout << value << std::endl;
}
//===================================================================================


//===================================================================================
void CPU::Execute(std::vector<int>& program)
{
	instructionPointer_ = 0;
	basePointer_        = 0;
	int currentCommand = CMD_EXIT;

	do
	{
		currentCommand = program[instructionPointer_];
		instructionPointer_++;

		switch (currentCommand)
		{
		#define DEF_CMD_(name, num, args, code)  \
		case num:                                \
			ExecuteCommand<name>(program);       \
			break;
		#include "..\CPU_commands.h"
		#undef DEF_CMD_
		default:
			break;
		}
	} 
	while (exitFlag_ != true);

	Dump();
}