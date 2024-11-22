#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
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

    try {
        manager.loadProcesses();
    } catch (const std::exception& e) {
        std::cerr << "Error loading processes: " << e.what() << std::endl;
        return 1;
    }

    std::string flag = (argc > 1) ? argv[1] : "-c"; // Default to CPU usage sorting

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

    return 0;
}
