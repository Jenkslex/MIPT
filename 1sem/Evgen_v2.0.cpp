#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

void FileInputStrings (std::string fileName, std::vector<std::string> &strings);
void FileOutputStrings(std::string fileName, std::vector<std::string> &strings);

int main()
{
	std::vector<std::string> strings;

	FileInputStrings("Poem.txt", strings);

	std::sort(strings.begin(), strings.end(), [&](std::string a, std::string b) { return a > b; });

	FileOutputStrings("Sorted.txt", strings);

	system("pause");
	return 0;
}

void FileInputStrings(std::string fileName, std::vector<std::string> &strings)
{
	std::string tempLine;

	std::ifstream fin (fileName);

	while (!fin.eof())
	{
		std::getline(fin, tempLine);
		strings.push_back(tempLine);
	}

	fin.close();
}

void FileOutputStrings(std::string fileName, std::vector<std::string> &strings)
{
	std::ofstream fout(fileName);

	for (auto it = strings.begin(); it != strings.end(); it++)
		fout << *it << std::endl;

	fout.close();
}