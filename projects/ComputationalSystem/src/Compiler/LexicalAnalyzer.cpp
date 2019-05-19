#include "LexicalAnalyzer.h"

#include <ctype.h>
#include <string>
#include <assert.h>


enum SymbolType 
{
	Space        = 0,
	Digit        = 1,
	OperatorPart = 2,
	WordPart     = 3,
	Bracket      = 4
};

SymbolType getType(char symbol);

bool isDigit   (char symbol);
bool isOperator(char symbol);
bool isBracket (char symbol);
bool isWordPart(char symbol);

std::string divideString(std::string::iterator& stringIterator, bool(*condition)(char));


LexicalAnalyzer::LexicalAnalyzer()
{
	FillDecodeTable();
}

LexicalAnalyzer::~LexicalAnalyzer()
{
}

std::vector<Lexeme> LexicalAnalyzer::Translate(std::string langProgram)
{
	std::vector<Lexeme> lexemes;
	Lexeme currLexeme(UnknownOperator);
	auto stringIterator = langProgram.begin();

	while (stringIterator != langProgram.end())
	{
		SymbolType symbol = getType(*stringIterator);

		switch (symbol)
		{
		case Space:        stringIterator++;                           continue;
		case Digit:        currLexeme = ParseNumber  (stringIterator); break;
		case OperatorPart: currLexeme = ParseOperator(stringIterator); break;
		case WordPart:     currLexeme = ParseWord    (stringIterator); break;
		case Bracket:      currLexeme = ParseBracket (stringIterator); break;
		default:
			throw std::runtime_error("Couldn't parse symbol " + 
				std::to_string(symbol) + "!");
			break;
		}

		lexemes.push_back(currLexeme);
	}

	return lexemes;
}

Operator LexicalAnalyzer::Decode(std::string code)
{
	auto it = decodeTable_.find(code);

	if (it == decodeTable_.end())
		return UnknownOperator;

	return it->second;
}

Lexeme LexicalAnalyzer::ParseNumber(std::string::iterator & stringIterator)
{
	std::string lexemeCode = divideString(stringIterator, isDigit);
	assert(lexemeCode != "");

	int number = std::stoi(lexemeCode);
	return Lexeme(number);
}

Lexeme LexicalAnalyzer::ParseOperator(std::string::iterator& stringIterator)
{
	std::string lexemeCode = divideString(stringIterator, isOperator);
	Operator    oper       = Decode(lexemeCode);

	if (oper == UnknownOperator)
		throw std::runtime_error("Unknown operator " + lexemeCode + " !");

	return Lexeme(oper);
}

Lexeme LexicalAnalyzer::ParseWord(std::string::iterator & stringIterator)
{
	std::string lexemeCode = divideString(stringIterator, isWordPart);
	Operator    oper = Decode(lexemeCode);

	if (oper == UnknownOperator)
		return Lexeme(lexemeCode);

	return Lexeme(oper);
}

Lexeme LexicalAnalyzer::ParseBracket(std::string::iterator & stringIterator)
{
	char bracket = *stringIterator;
	stringIterator++;

	std::string lexemeCode;
	lexemeCode.push_back(bracket);
	Operator    oper       = Decode(lexemeCode);

	if (oper == UnknownOperator)
		throw std::runtime_error("Unknown operator " + lexemeCode + " !");

	return Lexeme(oper);
}


std::string divideString(std::string::iterator& stringIterator, bool(*condition)(char))
{
	std::string newString;
	std::string::iterator stringBegin = stringIterator;

	while (condition(*stringIterator) == true)
		stringIterator++;

	newString.assign(stringBegin, stringIterator);

	return newString;
}

SymbolType getType(char symbol)
{
	if      (isspace   (symbol)) return Space;
	else if (isOperator(symbol)) return OperatorPart;
	else if (isBracket (symbol)) return Bracket;
	else if (isdigit   (symbol)) return Digit;
	else if (isalpha   (symbol)) return WordPart;
}

bool isDigit(char symbol)
{
	return isdigit(symbol);
}

bool isOperator(char symbol)
{
	switch (symbol)
	{
	case '+':	case '-':
	case '*':	case '/':
	case '%':	case '=':
	case '<':	case '>':
	case '^':	case ';':
	case '&':	case '|':
	case '!':	case ',':
		return true;

	default:
		return false;
	}
}

bool isBracket(char symbol)
{
	switch (symbol)
	{
	case '(':	case ')':
	case '{':	case '}':
	case '[':	case ']':
		return true;

	default:
		return false;
	}
}

bool isWordPart(char symbol)
{
	return isalpha(symbol) || isdigit(symbol);
}
