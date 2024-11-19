#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
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

    int choice = -1;
    bool quit = false;

    while (!quit) {
        std::cout << "\nSelect an option:\n";
        std::cout << "1. Display processes (sorted by CPU usage)\n";
        std::cout << "2. Display processes (sorted by Memory usage)\n";
        std::cout << "3. Display processes (sorted by Disk usage)\n";
        std::cout << "4. Display processes (sorted by Network usage)\n";
        std::cout << "5. Refresh process list\n";
        std::cout << "0. Quit\n";
        std::cout << "Choice: ";
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                manager.sortProcesses("cpu");
                displayProcesses(manager.getProcesses());
                break;
            case 2:
                manager.sortProcesses("memory");
                displayProcesses(manager.getProcesses());
                break;
            case 3:
                manager.sortProcesses("disk");
                displayProcesses(manager.getProcesses());
                break;
            case 4:
                manager.sortProcesses("network");
                displayProcesses(manager.getProcesses());
                break;
            case 5:
                manager.loadProcesses();
                std::cout << "Process list refreshed." << std::endl;
                break;
            case 0:
                quit = true;
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
        }
    }

    return 0;
}
