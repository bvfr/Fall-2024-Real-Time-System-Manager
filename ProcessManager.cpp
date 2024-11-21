#include "ProcessManager.h"
#include <iostream>
#include <algorithm>
#include <dirent.h>
#include <cstdlib>

// Constructor: Initialize and load processes
ProcessManager::ProcessManager() : currentPage(0) {
    loadProcesses();
}

// Get all processes
const std::vector<Process>& ProcessManager::getProcesses() const {
    return processes;
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

    for (auto& process : processes) {
        process.updateUsage();
    }
}

// Sort processes based on user-selected criteria
void ProcessManager::sortProcesses(const std::string& criteria) {
    if (criteria == "cpu") {
        std::sort(processes.begin(), processes.end(),
                 [](const Process& a, const Process& b) {
                     return a.getCpuUsage() > b.getCpuUsage();
                 });
    } else if (criteria == "memory") {
        std::sort(processes.begin(), processes.end(),
                 [](const Process& a, const Process& b) {
                     return a.getMemoryUsage() > b.getMemoryUsage();
                 });
    } else if (criteria == "disk") {
        std::sort(processes.begin(), processes.end(),
                 [](const Process& a, const Process& b) {
                     return a.getDiskUsage() > b.getDiskUsage();
                 });
    } else if (criteria == "network") {
        std::sort(processes.begin(), processes.end(),
                 [](const Process& a, const Process& b) {
                     return a.getNetworkUsage() > b.getNetworkUsage();
                 });
    }
}
