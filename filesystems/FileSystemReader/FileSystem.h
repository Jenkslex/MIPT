#pragma once

#include <string>
#include <vector>


class FileSystem {
public:
    FileSystem() = default;

    virtual void openImage(std::string& fName) = 0;

    virtual void listDir() = 0;
    virtual void catFile(std::string& fName) = 0;
    virtual void printWorkingDirectory() = 0;

protected:
    FILE* fd = nullptr;
};