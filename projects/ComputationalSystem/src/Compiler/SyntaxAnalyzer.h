#pragma once

#include <vector>

#include "Lexeme.h"
#include "SyntaxData.h"

#include "..\Utility\BinaryNode.h"


class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer();
	~SyntaxAnalyzer();

	BinaryNode<SyntaxData>* Parse(std::vector<Lexeme>& lexemes);
};

