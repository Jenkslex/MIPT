#pragma once

#include <string>
#include <vector>

std::string readTextFile(std::string pathName);
void writeTextFile(std::string pathName, std::string buffer);

void writeCPUProgFile(std::string pathName, std::vector<int> program);