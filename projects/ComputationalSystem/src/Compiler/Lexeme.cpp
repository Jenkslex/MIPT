#include "Lexeme.h"


Lexeme::Lexeme(Operator oper):
	type_(OperatorType),
	oper_(oper)
{
}

Lexeme::Lexeme(std::string string):
	type_(StringType),
	string_(string)
{
}

Lexeme::Lexeme(int number):
	type_(NumberType),
	number_(number)
{
}

Lexeme::~Lexeme()
{
}
