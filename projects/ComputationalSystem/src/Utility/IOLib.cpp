#include "IOLib.h"

#include <fstream>

std::string readTextFile(std::string pathName)
{
	std::ifstream fileStream;
	fileStream.open(pathName, std::fstream::in);

	if (!fileStream.is_open())
		throw std::runtime_error("Couldn't open the file!");

	std::string text;

	fileStream.seekg(0, std::ios::end);
	text.reserve(fileStream.tellg());
	fileStream.seekg(0, std::ios::beg);

	text.assign((std::istreambuf_iterator<char>(fileStream)),
		std::istreambuf_iterator<char>());

	fileStream.close();

	return text;
}

void writeTextFile(std::string pathName, std::string buffer)
{

}

void writeCPUProgFile(std::string pathName, std::vector<int> program)
{
	std::ofstream fileStream;
	fileStream.open(pathName, std::fstream::out);

	if (!fileStream.is_open())
		throw std::runtime_error("Couldn't open the file!");

	for (auto unit : program)
		fileStream << unit << " ";

	fileStream << std::endl;

	fileStream.close();
}
