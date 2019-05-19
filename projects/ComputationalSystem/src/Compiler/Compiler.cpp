#include "Compiler.h"

Compiler::Compiler()
{}

Compiler::~Compiler()
{}

std::string Compiler::CompileFromBuffer(std::string& langProgram)
{
	std::vector<Lexeme>     lexemes    = lexicalAnalizer_.Translate(langProgram);
	BinaryNode<SyntaxData>* syntaxTree = syntaxAnalyzer_.Parse(lexemes);

	return std::string();
}
