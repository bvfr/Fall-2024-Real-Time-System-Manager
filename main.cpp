#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <chrono>
#include <thread>
#include "ProcessManager.h"

void displayProcesses(const std::vector<Process>& processes, int startIndex = 0, int count = 10) {
    int endIndex = std::min(startIndex + count, static_cast<int>(processes.size()));
    for (int i = startIndex; i < endIndex; ++i) {
        processes[i].display();
    }
}

int main() {
    ProcessManager manager;

    try {
        manager.loadProcesses();
    } catch (const std::exception& e) {
        std::cerr << "Error loading processes: " << e.what() << std::endl;
        return 1;
    }

    int choice = 1; // Default to CPU usage sorting
    bool quit = false;

    while (!quit) {
        // Sort and display processes based on the current choice
        switch (choice) {
            case 1:
                manager.sortProcesses("cpu");
                break;
            case 2:
                manager.sortProcesses("memory");
                break;
            case 3:
                manager.sortProcesses("disk");
                break;
            case 4:
                manager.sortProcesses("network");
                break;
            default:
                break;
        }
        displayProcesses(manager.getProcesses());

        // Wait for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Check for user input
        if (std::cin.rdbuf()->in_avail() > 0) {
            std::cin >> choice;
            if (choice == 0) {
                quit = true;
            }
        }
    }

    return 0;
}
