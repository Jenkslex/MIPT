#pragma once

#include <vector>
#include <map>

#include "Lexeme.h"
#include "CompilerTables.h"

class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	std::vector<Lexeme> Translate(std::string langProgram);

private:
	void FillDecodeTable();

	Lexeme ParseNumber  (std::string::iterator& stringIterator);
	Lexeme ParseOperator(std::string::iterator& stringIterator);
	Lexeme ParseWord    (std::string::iterator& stringIterator);
	Lexeme ParseBracket (std::string::iterator& stringIterator);

	Operator Decode(std::string code);

	std::map<std::string, Operator> decodeTable_;
};

