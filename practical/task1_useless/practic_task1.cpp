#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    const std::string filename = argv[1];
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << '\n';
        return 1;
    }

    std::vector<std::pair<int, std::string> > tasks;

    // Read tasks from file
    int delay;
    std::string program;
    while (file >> delay >> program) {
        tasks.push_back(std::make_pair(delay, program));
    }

    // Sort tasks by delay
    std::sort(tasks.begin(), tasks.end());

    // Execute tasks
    auto start_time = std::chrono::steady_clock::now();
    for (const auto& task : tasks) {
        auto elapsed_time = std::chrono::steady_clock::now() - start_time;
        auto delay_time = std::chrono::seconds(task.first) - elapsed_time;
        if (delay_time > std::chrono::seconds(0)) {
            std::this_thread::sleep_for(delay_time);
        }
        system(task.second.c_str());
    }

    return 0;
}