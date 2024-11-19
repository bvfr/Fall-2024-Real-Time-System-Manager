#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // For sorting
#include "ProcessManager.h"

void displayProcesses(const std::vector<Process>& processes, int startIndex = 0, int count = 10) {
    // Display a set number of processes (default is 10) starting from startIndex
    int endIndex = std::min(startIndex + count, static_cast<int>(processes.size()));
    for (int i = startIndex; i < endIndex; ++i) {
        std::cout << "PID: " << processes[i].getPid()
                  << " | Name: " << processes[i].getProcessName()
                  << " | CPU: " << processes[i].getCpuUsage()
                  << "% | Memory: " << processes[i].getMemoryUsage()
                  << "MB | Disk: " << processes[i].getDiskUsage()
                  << "MB | Network: " << processes[i].getNetworkUsage()
                  << "MB" << std::endl;
    }
}

int main() {
    ProcessManager manager;
    manager.loadProcesses(); // Load the initial list of processes from the system

    int choice = -1;
    bool quit = false;

    while (!quit) {
        std::cout << "\nSelect an option:\n";
        std::cout << "1. Display processes (sorted by CPU usage)\n";
        std::cout << "2. Display processes (sorted by Memory usage)\n";
        std::cout << "3. Display processes (sorted by Disk usage)\n";
        std::cout << "4. Display processes (sorted by Network usage)\n";
        std::cout << "5. Find process by PID\n";
        std::cout << "6. Find processes by name\n";
        std::cout << "0. Quit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                manager.sortByCpuUsage();
                displayProcesses(manager.getProcesses());
                break;
            case 2:
                manager.sortByMemoryUsage();
                displayProcesses(manager.getProcesses());
                break;
            case 3:
                manager.sortByDiskUsage();
                displayProcesses(manager.getProcesses());
                break;
            case 4:
                manager.sortByNetworkUsage();
                displayProcesses(manager.getProcesses());
                break;
            case 5: {
                int pid;
                std::cout << "Enter PID: ";
                std::cin >> pid;
                Process* process = manager.findProcessByPid(pid);
                if (process) {
                    std::cout << "Process found: PID: " << process->getPid()
                              << " | Name: " << process->getProcessName()
                              << " | CPU: " << process->getCpuUsage()
                              << "% | Memory: " << process->getMemoryUsage()
                              << "MB | Disk: " << process->getDiskUsage()
                              << "MB | Network: " << process->getNetworkUsage()
                              << "MB" << std::endl;
                } else {
                    std::cout << "No process found with PID " << pid << std::endl;
                }
                break;
            }
            case 6: {
                std::string name;
                std::cout << "Enter process name: ";
                std::cin >> name;
                auto processes = manager.findProcessesByName(name);
                if (!processes.empty()) {
                    displayProcesses(processes);
                } else {
                    std::cout << "No processes found with name " << name << std::endl;
                }
                break;
            }
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

