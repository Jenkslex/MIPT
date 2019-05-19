#pragma once

#include <vector>

#include "FileSystem.h"

class Application {
public:
    Application() = default;

    void run();

private:
    void printHelp();
    void closeFS();

    std::vector<std::string> readCommand();

    FileSystem* fs = nullptr;
};