#pragma once

#include <string>

#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"

class Compiler
{
public:
	Compiler();
	~Compiler();

	std::string CompileFromBuffer(std::string& langProgram);

private:
	LexicalAnalyzer lexicalAnalizer_;
	SyntaxAnalyzer  syntaxAnalyzer_;
};