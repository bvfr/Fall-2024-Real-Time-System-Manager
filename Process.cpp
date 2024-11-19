#include "Process.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <dirent.h>

// Constructor initializing the PID, name, and statFilePath
Process::Process(int pid) : pid(pid), cpuUsage(0.0f), memoryUsage(0.0f),
                            diskUsage(0.0f), networkUsage(0.0f) {
    statFilePath = "/proc/" + std::to_string(pid) + "/stat";
    name = fetchProcessName();
}

// Fetch process name based on PID
std::string Process::fetchProcessName() const {
    std::ifstream cmdFile("/proc/" + std::to_string(pid) + "/comm");
    std::string processName;
    if (cmdFile) {
        std::getline(cmdFile, processName);
    }
    return processName.empty() ? "Unknown" : processName;
}

// Update all resource usage metrics
void Process::updateUsage() {
    cpuUsage = fetchCpuUsage();
    memoryUsage = fetchMemoryUsage();
    diskUsage = fetchDiskUsage();
    networkUsage = fetchNetworkUsage();
}

// Fetch CPU usage based on /proc/[pid]/stat and /proc/uptime
float Process::fetchCpuUsage() const {
    std::ifstream statFile(statFilePath);
    std::ifstream uptimeFile("/proc/uptime");
    std::string line;
    long utime = 0, stime = 0, cutime = 0, cstime = 0;
    long starttime = 0;
    float systemUptime = 0.0;

    if (uptimeFile) {
        uptimeFile >> systemUptime;
    }

    if (statFile && std::getline(statFile, line)) {
        std::istringstream iss(line);
        std::string dummy;
        for (int i = 1; i <= 22; ++i) {
            if (i == 14) iss >> utime;      // utime
            else if (i == 15) iss >> stime; // stime
            else if (i == 16) iss >> cutime; // cutime
            else if (i == 17) iss >> cstime; // cstime
            else if (i == 22) iss >> starttime; // starttime
            else iss >> dummy;
        }
    }

    long total_time = utime + stime + cutime + cstime;
    float seconds = systemUptime - (starttime / sysconf(_SC_CLK_TCK));
    if (seconds > 0) {
        return 100.0 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
    }

    return 0.0f;
}

// Fetch memory usage from /proc/[pid]/status
float Process::fetchMemoryUsage() const {
    std::ifstream statusFile("/proc/" + std::to_string(pid) + "/status");
    std::string line;
    float memoryUsageKB = 0.0f;

    while (std::getline(statusFile, line)) {
        if (line.find("VmRSS:") == 0) {
            std::istringstream iss(line);
            std::string key;
            iss >> key >> memoryUsageKB; // VmRSS is resident memory in KB
            break;
        }
    }

    return memoryUsageKB / 1024.0f; // Convert to MB
}

// Fetch disk usage from /proc/[pid]/io
float Process::fetchDiskUsage() const {
    std::ifstream ioFile("/proc/" + std::to_string(pid) + "/io");
    std::string line;
    long readBytes = 0;
    long writeBytes = 0;

    while (std::getline(ioFile, line)) {
        if (line.find("read_bytes:") == 0) {
            std::istringstream iss(line);
            std::string key;
            iss >> key >> readBytes;
        } else if (line.find("write_bytes:") == 0) {
            std::istringstream iss(line);
            std::string key;
            iss >> key >> writeBytes;
        }
    }

    long totalBytes = readBytes + writeBytes;
    return static_cast<float>(totalBytes) / (1024.0f * 1024.0f); // Convert to MB
}

// Helper function to check if a file descriptor is a network socket
bool isNetworkSocket(const std::string& fdPath) {
    char linkTarget[256];
    ssize_t len = readlink(fdPath.c_str(), linkTarget, sizeof(linkTarget) - 1);
    if (len != -1) {
        linkTarget[len] = '\0';
        std::string targetStr(linkTarget);
        // Sockets are usually represented by "socket:[XXXX]" in /proc/[pid]/fd/
        return targetStr.find("socket:[") != std::string::npos;
    }
    return false;
}

// Implementation of fetchNetworkUsage
float Process::fetchNetworkUsage() const {
    std::string fdDirPath = "/proc/" + std::to_string(pid) + "/fd/";
    DIR* dir = opendir(fdDirPath.c_str());
    if (!dir) {
        std::cerr << "Failed to open file descriptor directory for process " << pid << std::endl;
        return 0.0f;
    }

    struct dirent* entry;
    int socketCount = 0;

    while ((entry = readdir(dir)) != nullptr) {
        std::string fdPath = fdDirPath + entry->d_name;
        if (isNetworkSocket(fdPath)) {
            socketCount++;
        }
    }

    closedir(dir);

    // Placeholder estimation: Multiply socket count by a fixed bandwidth usage estimate
    // This can be refined by more complex monitoring if needed.
    const float estimatedUsagePerSocket = 0.5f; // Assume each socket ~0.5MB/sec usage
    float estimatedNetworkUsage = socketCount * estimatedUsagePerSocket;

    return estimatedNetworkUsage; // Return estimated usage in MB
}

// Accessor methods
int Process::getPid() const { return pid; }
std::string Process::getName() const { return name; }
float Process::getCpuUsage() const { return cpuUsage; }
float Process::getMemoryUsage() const { return memoryUsage; }
float Process::getDiskUsage() const { return diskUsage; }
float Process::getNetworkUsage() const { return networkUsage; }

