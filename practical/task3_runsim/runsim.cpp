#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " N" << std::endl;
        return 1;
    }

    const int N = std::atoi(argv[1]);
    int running = 0;
    std::string command;

    while (std::getline(std::cin, command)) {
        if (running >= N) {
            std::cerr << "Too many running commands" << std::endl;
            break;
        }

        pid_t pid = fork();
        if (pid == -1) {
            std::cerr << "Fork error" << std::endl;
            break;
        } else if (pid == 0) {
            // Child process
            system(command.c_str());
            return 0;
        } else {
            // Parent process
            running++;
            int status;
            waitpid(pid, &status, 0);
            running--;
        }
    }

    return 0;
}