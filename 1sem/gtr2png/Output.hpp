#include "SyntaxTree.hpp"
#include "Settings.hpp"
#include "FileException.hpp"
#include "Exception.hpp"


const char* GetInstructionName(int aOpcode);
void OutputNodeData(FILE* aFile, CSyntaxNode* aNode);
void OutputNode(FILE* aFile, CSyntaxNode* aNode);
void OutputTree(const char* aFileName, CSyntaxTree& aTree);
void OutputString(const char* aString, FILE* aOutput);

