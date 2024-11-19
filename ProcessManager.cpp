#include "ProcessManager.h"
#include <iostream>
#include <algorithm>
#include <dirent.h>
#include <cstdlib>

// Constructor: Initialize and load processes
ProcessManager::ProcessManager() : currentPage(0) {
    loadProcesses();
}

// Load all processes from the system
void ProcessManager::loadProcesses() {
    processes.clear();
    DIR* procDir = opendir("/proc");
    
    if (!procDir) {
        std::cerr << "Error: Unable to open /proc directory" << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        if (isdigit(entry->d_name[0])) {
            try {
                int pid = std::stoi(entry->d_name);
                processes.emplace_back(pid);
            } catch (const std::exception& e) {
                std::cerr << "Error processing PID " << entry->d_name << ": " << e.what() << std::endl;
                continue;
            }
        }
    }
    closedir(procDir);
}

// Refresh all processes (update resource usage)
void ProcessManager::refreshProcesses() {
    for (auto& process : processes) {
        process.updateUsage();
    }
}

// Display 10 processes per page
void ProcessManager::displayProcesses() const {
    size_t startIndex = currentPage * pageSize;
    size_t endIndex = std::min(startIndex + pageSize, processes.size());

    for (size_t i = startIndex; i < endIndex; ++i) {
        processes[i].display();
    }
}

// Sort processes based on user-selected criteria
void ProcessManager::sortProcesses(const std::string& criteria) {
    if (criteria == "cpu") {
        sortByCpuUsage();
    } else if (criteria == "memory") {
        sortByMemoryUsage();
    } else if (criteria == "disk") {
        sortByDiskUsage();
    } else if (criteria == "network") {
        sortByNetworkUsage();
    }
}

// Pagination - Show next page
void ProcessManager::showNextPage() {
    if ((currentPage + 1) * pageSize < processes.size()) {
        currentPage++;
    }
}

// Pagination - Show previous page
void ProcessManager::showPreviousPage() {
    if (currentPage > 0) {
        currentPage--;
    }
}

// Get all processes
const std::vector<Process>& ProcessManager::getProcesses() const {
    return processes;
}

// Sorting methods
void ProcessManager::sortByCpuUsage() {
    std::sort(processes.begin(), processes.end(),
              [](const Process& a, const Process& b) {
                  return a.getCpuUsage() > b.getCpuUsage();
              });
}

void ProcessManager::sortByMemoryUsage() {
    std::sort(processes.begin(), processes.end(),
              [](const Process& a, const Process& b) {
                  return a.getMemoryUsage() > b.getMemoryUsage();
              });
}

void ProcessManager::sortByDiskUsage() {
    std::sort(processes.begin(), processes.end(),
              [](const Process& a, const Process& b) {
                  return a.getDiskUsage() > b.getDiskUsage();
              });
}

void ProcessManager::sortByNetworkUsage() {
    std::sort(processes.begin(), processes.end(),
              [](const Process& a, const Process& b) {
                  return a.getNetworkUsage() > b.getNetworkUsage();
              });
}
