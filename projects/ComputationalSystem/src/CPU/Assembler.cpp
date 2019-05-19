#include "Assembler.h"

#include "..\Utility\IOLib.h"

std::map<std::string, int> registerMap = {
	{ "ax", 0 },
	{ "bx", 1 },
	{ "cx", 2 },
	{ "dx", 3 }
};

Assembler::Assembler()
{
	FillTables();
}


Assembler::~Assembler()
{
}

void Assembler::FillTables()
{
	#define DEF_CMD_(name, num, args, code)  decodeTable[#code] = name;
	#include "..\CPU_commands.h"
	#undef DEF_CMD_

	#define DEF_CMD_(name, num, args, code)  argumentTable[name] = (ArgumentType)args;
	#include "..\CPU_commands.h"
	#undef DEF_CMD_
}

/*
std::vector<int>& Assembler::AssemblyFromStream(std::istream stream)
{

}

std::vector<int>& Assembler:: AssemblyFromBuffer(std::string program)
{

}*/

std::vector<int> Assembler::AssemblyFromFile(std::string pathName)
{
	std::string ASM_program = readTextFile(pathName);
	std::vector<int> CPU_program;
	labelAddresses.clear();

	Pass(CPU_program, ASM_program);
	Pass(CPU_program, ASM_program);

	return CPU_program;
}

void Assembler::Pass(std::vector<int>& CPU_program, std::string& ASM_program)
{
	CPU_program.clear();

	std::stringstream inbuf(ASM_program);

	std::string currentCommandAsm;
	std::string ASM_argument;
	CPU_Command currentCommandCPU = CMD_EXIT;
	int argument = 0;

	do
	{
		inbuf >> currentCommandAsm;

		if (currentCommandAsm.back() == ':')
		{
			AddLabelAddress(currentCommandAsm, CPU_program.size());
			continue;
		}

		currentCommandCPU = Decode(currentCommandAsm);
		CPU_program.push_back(currentCommandCPU);

		ArgumentType argType = GetArgumentType(currentCommandCPU);

		if (argType != noArgument)
		{
			inbuf >> ASM_argument;

			switch (argType)
			{
			case numArgument:   argument = GetNumericalArgument(ASM_argument); break;
			case labelArgument: argument = GetLabelAddress     (ASM_argument); break;
			case regArgument:   argument = GetRegisterNumber   (ASM_argument); break;
			}

			CPU_program.push_back(argument);
		}
	} while (currentCommandCPU != CMD_EXIT);
}

CPU_Command Assembler::Decode(std::string ASM_command)
{
	auto it = decodeTable.find(ASM_command);

	if (it == decodeTable.end())
		throw std::runtime_error("Wrong command name " + 
			ASM_command + "!");

	return it->second;
}

ArgumentType Assembler::GetArgumentType(CPU_Command command)
{
	return argumentTable[command];
}

int Assembler::GetRegisterNumber(std::string ASM_register)
{
	auto it = registerMap.find(ASM_register);

	if (it == registerMap.end())
		throw std::runtime_error("Wrong register name " + 
			ASM_register + "!");

	return it->second;
}

int Assembler::GetNumericalArgument(std::string numberInString)
{
	try {
		int number = std::stoi(numberInString);
		return number;
	}
	catch (std::invalid_argument& e) {
		throw std::runtime_error("Excepted number, but " + 
			numberInString + " is not a number!");
	}
}

void Assembler::AddLabelAddress(std::string label, int address)
{
	label.pop_back();

	if (label == "")
		throw std::runtime_error("Empty label by address " + 
			std::to_string(address) + "!");

	labelAddresses[label] = address;
}

int Assembler::GetLabelAddress(std::string label)
{
	auto it = labelAddresses.find(label);

	if (it != labelAddresses.end())
		return it->second;

	return 0;
}

