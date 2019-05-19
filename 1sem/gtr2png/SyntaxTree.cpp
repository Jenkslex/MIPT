#include "SyntaxTree.hpp"

CSyntaxTree::CSyntaxTree(const char *aFileName)
{
	//TODO: close file properly if CSyntaxNode throws
	FILE* File = fopen(aFileName, "rt");

	if (!File) __THROW("Could not open %s to read a syntax tree.", aFileName);

	char Temp[80] = "";
	float Version = 0.0;

	fscanf(File, "%s %f", Temp, &Version);
	if (strcmp(Temp, "~GTR"))
	{
		throw CFileException("Invalid file flag (\"~GTR\" expected).", File);
	}
	if (Version > GTR_VERSION)
	{
		throw CFileException("Invalid GTR format version.", File);
	}
	fscanf(File, "%s ", Temp);
	if (!strcmp("node_start", Temp))
	{
		mCoreNode = new CSyntaxNode(File);
	}
	else
	{		
		throw CFileException("Program must have a core node.", File);
	}

	fclose(File);
}

CSyntaxTree::~CSyntaxTree()
{
	delete mCoreNode;
}

void CSyntaxNode::ReadStringConstant(FILE* aFile)
{
	mData.Type = EStringConstant;
	if (fgetc(aFile) != '"') throw CFileException("Invalid string beginning ('\"' expected).", aFile);
	bool Ended = false;
	while (!Ended)
	{
		if (feof(aFile)) throw CFileException("EOF in string constant.", aFile);
		char Character = fgetc(aFile);
		switch (Character)
		{
		case '\\':
			if (feof(aFile)) throw CFileException("EOF in string constant.", aFile);
			switch (fgetc(aFile))
			{
			case '"':
				Data().StringConstant.push_back('"');
				break;
			case 'n':
				Data().StringConstant.push_back('\n');
				break;
			default:
				throw CFileException("Invalid escape-sequence in string.", aFile);
				break;
			};
			break;
		case '\n':
			throw CFileException("New line in string constant.", aFile);
			break;
		case '"':
			Ended = true;
			break;
		default:
			Data().StringConstant.push_back(Character);
			break;
		};
	}
}

CSyntaxNode::CSyntaxNode(FILE *aFile)
{
	char Temp[80];
	fscanf(aFile, "%s ", Temp);
	string Type = Temp;
	if (Type == "instruction")
	{
		mData.Type = EInstruction;
		fscanf(aFile, "%d ", (int*) &mData.Instruction);
	}
	else if (Type == "number")
	{
		mData.Type = ENumberConstant;
		fscanf(aFile, "%f ", &mData.NumberConstant);
	}
	else if (Type == "string")
	{
		ReadStringConstant(aFile);
	}
	else if (Type == "variable")
	{
		mData.Type = EVariable;
		fscanf(aFile, "%s ", Temp);
		mData.Identifier = Temp;
	}
	else if (Type == "label")
	{
		mData.Type = ELabel;
		fscanf(aFile, "%s ", Temp);
		mData.Identifier = Temp;
	}
	else if (Type == "new_variable")
	{
		mData.Type = ENewVariable;
		fscanf(aFile, "%s ", Temp);
		mData.Identifier = Temp;
	}
	else if (Type == "new_label")
	{
		mData.Type = ENewLabel;
		fscanf(aFile, "%s ", Temp);
		mData.Identifier = Temp;
	}
	else if (Type == "blank")
	{
		mData.Type = EBlank;
	}
	else if (Type == "new_function")
	{
		mData.Type = ENewFunction;
		fscanf(aFile, "%s ", Temp);
		mData.Identifier = Temp;
	}
	else if (Type == "parameter")
	{
		mData.Type = EParameter;
		fscanf(aFile, "%s ", Temp);
		mData.Identifier = Temp;
	}
	else if (Type == "parameter_value")
	{
		mData.Type = EParameterValue;
	}
	else if (Type == "function")
	{
		mData.Type = EFunction;
		fscanf(aFile, "%s ", Temp);
		mData.Identifier = Temp;
	}
	else
	{
		throw CFileException("Invalid node type", aFile);
	}

	fscanf(aFile, "%s ", Temp);
	if (!strcmp(Temp, "node_start"))
	{
		CSyntaxNode* Child = new CSyntaxNode(aFile);
		SetLeft(Child);
	}
	else if (!strcmp(Temp, "node_end")) return;
	else throw CFileException("Invalid token (\"node_start\" or \"node_end\" expected).", aFile);

	fscanf(aFile, "%s ", Temp);
	if (!strcmp(Temp, "node_start"))
	{
		CSyntaxNode* Child = new CSyntaxNode(aFile);
		SetRight(Child);
	}
	else if (!strcmp(Temp, "node_end")) return;
	else throw CFileException("Invalid token (\"node_start\" or \"node_end\" expected).", aFile);

	fscanf(aFile, "%s ", Temp);
	if (!strcmp(Temp, "node_end")) return;
	else throw CFileException("Invalid token (\"node_end\" expected).", aFile);
}

CSyntaxNode::CSyntaxNode()
{
	mData.Type = EBlank;
}

CSyntaxNode::CSyntaxNode(float aNumberConstant)
{
	mData.Type				= ENumberConstant;
	mData.NumberConstant	= aNumberConstant;
}

CSyntaxNode::CSyntaxNode(string aStringConstant)
{
	mData.Type				= EStringConstant;
	mData.StringConstant	= aStringConstant;
}

CSyntaxNode::CSyntaxNode(TNodeType aType, string aIdentifier)
{
	if (aType != EVariable		&& aType != ELabel &&
		aType != ENewVariable	&& aType != ENewLabel)
	{
		throw "Invalid CSyntaxNode use (INTERNAL ERROR).";
	}
	mData.Type			= aType;
	mData.Identifier	= aIdentifier;
}

CSyntaxNode::CSyntaxNode(TInstruction aInstruction)
{
	mData.Type			= EInstruction;
	mData.Instruction	= aInstruction;
}


void CSyntaxNode::Output(FILE* aFile)
{
	fprintf(aFile, "\nnode_start\n");
	switch (mData.Type)
	{
	case EInstruction:
		fprintf(aFile, "instruction %d\n", (int) mData.Instruction);
		break;
	case ENumberConstant:
		fprintf(aFile, "number %lg\n", mData.NumberConstant);
		break;
	case EStringConstant:
		fprintf(aFile, "string \"%s\"\n", mData.StringConstant.c_str());
		break;
	case EVariable:
		fprintf(aFile, "variable %s\n", mData.Identifier.c_str());
		break;
	case ELabel:
		fprintf(aFile, "label %s\n", mData.Identifier.c_str());
		break;
	case ENewVariable:
		fprintf(aFile, "new_variable %s\n", mData.Identifier.c_str());
		break;
	case ENewLabel:
		fprintf(aFile, "new_label %s\n", mData.Identifier.c_str());
		break;
	case EBlank:
		fprintf(aFile, "blank\n");	
		break;
	case ENewFunction:
		fprintf(aFile, "new_function %s\n", mData.Identifier.c_str());
		break;
	case EFunction:
		fprintf(aFile, "function %s\n", mData.Identifier.c_str());
		break;
	case EParameter:
		fprintf(aFile, "parameter %s\n", mData.Identifier.c_str());
		break;
	case EParameterValue:
		fprintf(aFile, "parameter_value\n", mData.Identifier.c_str());
		break;
		
	default:
		__THROW("Invalid node type (%d).", mData.Type);
	};
	if (mLeft)  Left()->Output(aFile);
	if (mRight) Right()->Output(aFile);	
	fprintf(aFile, "node_end\n\n");
}

void CSyntaxTree::Output(const char *aFileName)
{	
	FILE* Output = fopen(aFileName, "wt");
	if (!Output) __THROW("Error opening %s for writing a tree.\n", aFileName);
	fprintf(Output, "~GTR %lg\n\n", (float) GTR_VERSION);
	mCoreNode->Output(Output);
	fclose(Output);
}

CSyntaxTree::CSyntaxTree(CSyntaxNode* aCoreNode) :
mCoreNode(aCoreNode)
{
}