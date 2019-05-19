#include "Include.h"

#include "Utility\IOLib.h"

#include "CPU\Assembler.h"
#include "CPU\CPU.h"
#include "Compiler\Compiler.h"

int main()
{
	std::string pathName = "../bin/LangProg.txt";

	Compiler  compiler;
	Assembler assembler;
	CPU       cpu;

	try 
	{
		std::string langProgram = readTextFile(pathName);
		std::string asmProgram  = compiler.CompileFromBuffer(langProgram);
	}
	catch (const std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
		cpu.Dump();
	}

	system("pause");
	return 0;
}