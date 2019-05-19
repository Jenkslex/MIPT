#include "Application.h"

#include <iostream>
#include <iterator>
#include <sstream>

#include "Fat16FileSystem.h"

void Application::run() {
    printHelp();

    do {
        auto command = readCommand();

        if (command.empty())
            continue;

        else if (command[0] == "help")
            printHelp();

        else if (command[0] == "exit") {
            closeFS();
            break;
        }
        else if (command[0] == "fat16" || command[0] == "ext2" &&
                command.size() == 2) {

            if (command[0] == "fat16")
                fs = reinterpret_cast<FileSystem*>(new Fat16FileSystem());
            else
                fs = nullptr;

            try{
                fs->openImage(command[1]);
            }
            catch (std::runtime_error& error) {
                std::cout << error.what() << std::endl;
                closeFS();
            }
        }
        else if (command[0] == "ls") {
            if(fs)
                fs->listDir();
        }
        else if (command[0] == "cat" && command.size() == 2) {
            if(fs)
                fs->catFile(command[1]);
        }


    }
    while (true);
}

std::vector<std::string> Application::readCommand() {
    std::cout << ">";
    std::string command;
    std::getline(std::cin, command);

    std::istringstream iss(command);
    std::vector<std::string> commands{std::istream_iterator<std::string>{iss},
                                      std::istream_iterator<std::string>{}};

    return commands;
}

void Application::closeFS() {
    if (fs != nullptr)
        delete fs;
}


void Application::printHelp() {
    std::cout << "This is FileSystem reader v0.1"          << std::endl;
    std::cout << "Usage: "                                 << std::endl;
    std::cout << "help          - Shows this help page"    << std::endl;
    std::cout << "exit          - Close program"           << std::endl;
    std::cout <<                                              std::endl;
    std::cout << "fat16 <fName> - Open image of fat16 fs"  << std::endl;
    std::cout << "ext2  <fName> - Open image of ext2  fs"  << std::endl;
    std::cout <<                                              std::endl;
    std::cout << "ls            - List current directory"  << std::endl;
    std::cout << "pwd           - Print current directory" << std::endl;
}