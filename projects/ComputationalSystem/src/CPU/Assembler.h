#pragma once

#include "..\Include.h"
#include "Tables.h"

enum ArgumentType
{
	noArgument    = 0,
	numArgument   = 1,
	labelArgument = 2,
	regArgument   = 3
};

class Assembler
{
public:
	Assembler();
	~Assembler();

	//std::vector<int>& AssemblyFromStream (std::istream stream);
	//std::vector<int>& AssemblyFromBuffer (std::string program);
	std::vector<int> AssemblyFromFile   (std::string pathName);

private:
	void FillTables();

	void Pass(std::vector<int>& CPU_program, std::string& ASM_program);
	
	CPU_Command Decode(std::string ASM_command);
	ArgumentType GetArgumentType(CPU_Command command);

	int GetRegisterNumber(std::string ASM_register);
	int GetNumericalArgument(std::string numberInString);

	void AddLabelAddress(std::string label, int address);
	int GetLabelAddress(std::string label);

	std::map<std::string, CPU_Command>  decodeTable;
	std::map<CPU_Command, ArgumentType> argumentTable;

	std::map<std::string, int> labelAddresses;
};

