#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include "ProcessManager.h"

void displayProcesses(const std::vector<Process>& processes) {
    for (const auto& process : processes) {
        process.display();
    }
}

int main(int argc, char* argv[]) {
    ProcessManager manager;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [-c | -m | -d | -n]" << std::endl;
        return 1;
    }

    std::string flag = argv[1];

    while (true) {
        try {
            manager.loadProcesses();
        } catch (const std::exception& e) {
            std::cerr << "Error loading processes: " << e.what() << std::endl;
            return 1;
        }

        if (flag == "-c") {
            manager.sortProcesses("cpu");
        } else if (flag == "-m") {
            manager.sortProcesses("memory");
        } else if (flag == "-d") {
            manager.sortProcesses("disk");
        } else if (flag == "-n") {
            manager.sortProcesses("network");
        } else {
            std::cerr << "Invalid flag. Use -c, -m, -d, or -n." << std::endl;
            return 1;
        }

        displayProcesses(manager.getProcesses());

        // Wait for 1.5 seconds before refreshing
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    return 0;
}
