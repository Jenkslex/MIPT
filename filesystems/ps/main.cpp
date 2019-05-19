#include <iostream>
#include <dirent.h>
#include <fstream>
#include <vector>

class Process {
public:
    explicit Process(int pid) :
            Pid(pid)
    {
        std::string fPath = "/proc/" + std::to_string(pid) + "/comm";

        std::ifstream f(fPath);
        if(!f.fail()) {
            f >> Name;
            f.close();
        }
        else {
            std::cerr << "Cannot open file " << fPath << std::endl;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Process& proc);

private:
    int Pid;
    std::string Name;
};

std::ostream &operator<<(std::ostream &os, const Process &proc) {
    os << proc.Pid << "\t\t\t" << proc.Name;
    return os;
}


int main() {
    std::vector<Process> processes;

    DIR* dir = opendir ("/proc");
    if (!dir) {
        std::cerr << "Cannot open /proc \n";
        exit(1);
    }

    struct dirent* ent;
    std::string pid;

    while ((ent = readdir (dir)) != nullptr) {
        pid = ent->d_name;

        try {
            int pid_int = std::stoi(pid);
            processes.emplace_back(Process(pid_int));
        }
        catch (std::invalid_argument& e)
        {}
    }
    closedir (dir);

    for (auto& proc : processes)
        std::cout << proc << std::endl;

    return 0;
}