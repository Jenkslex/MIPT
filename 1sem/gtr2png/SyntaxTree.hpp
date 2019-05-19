#ifndef __SYNTAXTREE__
#define __SYNTAXTREE__

#define _CRT_SECURE_NO_WARNINGS

#include "BinaryTree.hpp"
#include <stdio.h>
#include <string>
#include "FileException.hpp"
#include "Exception.hpp"

#define GTR_VERSION 1.1f

using std::string;

enum TInstruction
{
	EAddition					= 0,
	ESubtraction				= 1,
	EMultiplication				= 2,
	EDivision					= 3,
	EModulo						= 4,
	EAssignment					= 5,

	ESin						= 60,
	ECos						= 61,
	EAsin						= 62,
	EAcos						= 63,
	ELn							= 64,
	ELog						= 65,
	EPow						= 66,
	ESqrt						= 67,
	EAbs						= 68,
	EWholePart					= 69,

	EInputNumber			    = 90,
	EOutputNumber			    = 91,
	EOutputString				= 92,
	EExit						= 93,
	EJump						= 94,
	EBranch						= 95,
	EIf							= 96,
	EWhile						= 97,
	EFunctionCall				= 96,

	EEqual						= 150,
	ENotEqual					= 151,
	ELessThanOrEqual			= 152,
	EGreaterThanOrEqual			= 153,
	ELessThan					= 154,
	EGreaterThan				= 155,
	ELogicalAnd					= 156,
	ELogicalOr					= 157,
	ELogicalNot					= 158,

/*Non-standard stuff goes here*/
	EMin						= 180,
	EMax,
	ENop,
	EIncrement,
	EDecrement,
	EDoWhile,
	EDiff
};

enum TNodeType
{
	EInstruction,
	ENumberConstant,
	EStringConstant,
	EVariable,
	ELabel,
	ENewVariable,
	ENewLabel,
	EBlank,
	ENewFunction,
	EParameter,
	EParameterValue,
	EFunction
};

struct TNodeData
{
	TNodeType Type;

	string			Identifier;
	string			StringConstant;
	float			NumberConstant;
	TInstruction	Instruction;
};



class CSyntaxNode : public cNode<TNodeData>
{
public:
	CSyntaxNode();
	CSyntaxNode(TInstruction aInstruction);
	CSyntaxNode(TNodeType aType, string aIdentifier);
	CSyntaxNode(string aStringConstant);
	CSyntaxNode(float aNumberConstant);
	CSyntaxNode(FILE* aInput);

	void ReadStringConstant(FILE* aFile);

	TNodeType		Type()				{return Data().Type;}
	TInstruction	Instruction()		{return Data().Instruction;}
	string			Identifier()		{return Data().Identifier;}
	string			StringConstant()	{return Data().StringConstant;}
	float			NumberConstant()	{return Data().NumberConstant;}
	CSyntaxNode*	Left()				{return (CSyntaxNode*) cNode<TNodeData>::Left();}
	CSyntaxNode*	Right()				{return (CSyntaxNode*) cNode<TNodeData>::Right();}

	void Output(FILE* aFile);
	void Simplify();
    
};

class CSyntaxTree
{
private:
    CSyntaxNode*        mCoreNode;

public:
	CSyntaxTree(CSyntaxNode* aCoreNode);
    CSyntaxTree(const char* aFileName);
	~CSyntaxTree();
    
	void Output(const char* aFileName);
    CSyntaxNode* CoreNode(){return mCoreNode;}
};

#endif