#include "ProcessManager.h"
#include <iostream>
#include <algorithm>
#include <dirent.h>
#include <cstdlib>
#include <fstream>

ProcessManager::ProcessManager() : currentPage(0) {
    loadProcesses();
}

const std::vector<Process>& ProcessManager::getProcesses() const {
    return processes;
}

void ProcessManager::loadProcesses() {
    processes.clear();
    DIR* procDir = opendir("/proc");

    if (!procDir) {
        std::cerr << "Error: Unable to open /proc directory" << std::endl;
        return;
    }

    struct dirent* entry;
    int count = 0; // Counter to track the number of processes loaded
    while ((entry = readdir(procDir)) != nullptr && count < 10) {
        if (isdigit(entry->d_name[0])) {
            try {
                int pid = std::stoi(entry->d_name);
                processes.emplace_back(pid);
                count++; // Increment the counter
            } catch (const std::exception& e) {
                continue;
            }
        }
    }
    closedir(procDir);

    for (auto& process : processes) {
        process.updateUsage();
    }
}

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
