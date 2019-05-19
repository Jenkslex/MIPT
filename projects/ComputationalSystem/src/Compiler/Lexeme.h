#pragma once

#include <string>

#include "CompilerTables.h"

enum LexemeType
{
	OperatorType = 0,
	StringType   = 1,
	NumberType   = 2
};


class Lexeme
{
public:
	Lexeme(Operator oper);
	Lexeme(std::string string);
	Lexeme(int number);
	~Lexeme();

private:
	LexemeType  type_;

	Operator    oper_;
	std::string string_;
	int         number_;
};

