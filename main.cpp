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

void displayInstructions() {
    std::cout << "\nInstructions:\n";
    std::cout << "1. Sort by CPU usage\n";
    std::cout << "2. Sort by Memory usage\n";
    std::cout << "3. Sort by Disk usage\n";
    std::cout << "4. Sort by Network usage\n";
    std::cout << "5. Refresh process list\n";
    std::cout << "0. Quit\n";
    std::cout << "Enter your choice: ";
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
    std::string input;

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
            case 5:
                manager.loadProcesses();
                break;
            default:
                break;
        }
        displayProcesses(manager.getProcesses());
        displayInstructions();

        // Wait for 1.5 seconds
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));

        // Check for user input
        if (std::cin.rdbuf()->in_avail() > 0) {
            std::getline(std::cin, input);
            if (!input.empty()) {
                choice = std::stoi(input);
                if (choice == 0) {
                    quit = true;
                }
            }
        }
    }

    return 0;
}
