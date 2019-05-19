#include "Output.hpp"

const char* GetInstructionName(int aOpcode)
{
	if (aOpcode < 256)
	{
		return OperationNames[aOpcode];
	}
	else __THROW("Too big instruction number (%d)", aOpcode);
}

void OutputString(const char* aString, FILE* aOutput)
{
	while (*aString)
	{
		switch (*aString)
		{
		case '\n':
			fprintf(aOutput, "\\\\n");
			break;
		default:
			fputc(*aString, aOutput);
			break;
		};

		aString++;
	}
}

void OutputNodeData(FILE* aFile, CSyntaxNode* aNode)
{
    fprintf(aFile, "node%p[label=\"", aNode);
    const char* Format = 0;
    TNodeData Data = aNode->Data();
	switch (Data.Type)
	{
	case EInstruction:
		fprintf(aFile, "instruction %d\\n[ %s ]",	(int) Data.Instruction, 
			GetInstructionName((int) Data.Instruction));
		Format = StyleInstruction;
		break;
	case ENumberConstant:
		fprintf(aFile, "number %lg",			Data.NumberConstant);
		Format = StyleNumberConstant;
		break;
	case EStringConstant:
		fprintf(aFile, "string \\\"");
		OutputString(Data.StringConstant.c_str(), aFile);
		fprintf(aFile, "\\\"");
		Format = StyleStringConstant;
		break;
	case EVariable:
		fprintf(aFile, "variable %s",			Data.Identifier.c_str());
		Format = StyleVariable;
		break;
	case ELabel:
		fprintf(aFile, "label %s",				Data.Identifier.c_str());
		Format = StyleLabel;
		break;
	case ENewVariable:
		fprintf(aFile, "new_variable %s",		Data.Identifier.c_str());
		Format = StyleNewVariable;
		break;
	case ENewLabel:
		fprintf(aFile, "new_label %s",			Data.Identifier.c_str());
		Format = StyleNewLabel;
		break;
	case EBlank:
		fprintf(aFile, "blank");
		Format = StyleBlank;
		break;
	case ENewFunction:
		fprintf(aFile, "new_function %s",		Data.Identifier.c_str());
		Format = StyleNewFunction;
		break;
	case EFunction:
		fprintf(aFile, "function %s",	    	Data.Identifier.c_str());
		Format = StyleFunction;
		break;
	case EParameter:
		fprintf(aFile, "parameter %s",			Data.Identifier.c_str());
		Format = StyleParameter;
		break;
	case EParameterValue:
		fprintf(aFile, "parameter_value");	
		Format = StyleParameterValue;
		break;
	default:
		__THROW("Invalid node type (%d).");
	};
	fprintf(aFile, "\", %s];\n", Format);
}

void OutputNode(FILE* aFile, CSyntaxNode* aNode)
{
    OutputNodeData(aFile, aNode);   
    
    if (aNode->Left())
    {
        fprintf (aFile, "\"node%p\" -> \"node%p\";\n", aNode, aNode->Left());
        OutputNode(aFile, aNode->Left());
    }
    if (aNode->Right())
    {
        fprintf(aFile, "\"node%p\" -> \"node%p\";\n", aNode, aNode->Right());
        OutputNode(aFile, aNode->Right());
    }
}

void OutputTree(const char* aFileName, CSyntaxTree& aTree)
{
    FILE* File = fopen(aFileName, "wt");
	if (!File) __THROW("Failed to open %s for tree output.", aFileName);
    fputs ("digraph g {\n", File);
    fprintf(File, "node_info[label=\"—генерировано программой gtr2png %s\", shape=rect, color=black, style=bold];\n", GTR2PNG_VERSION);
    OutputNode(File, aTree.CoreNode());
    fputs ("}", File);
    fclose(File);
}