#include "disk.h"
#include "utils.h"
#include <iostream>
#include <sys/statvfs.h>
#include <cstdlib>
#include <string>
#include <array>
#include <memory>
#include <stdexcept>
#include <chrono>
#include <regex>
#include <fstream>  // For ifstream
#include <sstream>  // For stringstream
#include <thread>   // For std::this_thread::sleep_for
#include <chrono>   // For std::chrono::seconds
#include <filesystem> // For C++17 filesystem
#include <csignal> // For signal handling
#include <atomic> // Include this for std::atomic

namespace fs = std::filesystem; // Define fs alias

std::atomic<bool> running(true);  // Global flag to control the loop

// Signal handler function to set the flag when a signal is received (e.g., Ctrl+C)
void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nProgram interrupted. Exiting...\n";
        running = false;  // Set flag to false to exit the loop
    }
}

long Disk::getDiskIOTime(const std::string& diskName) {
    std::ifstream file("/proc/diskstats");
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string dev;
        std::string discard;
        long io_time;

        // Parse the line to find the device stats
        ss >> discard >> discard >> dev;
        if (dev == diskName) {
            for (int i = 0; i < 9; ++i) ss >> discard; // Skip to the 10th field
            ss >> io_time; // I/O time in milliseconds
            return io_time;
        }
    }
    throw std::runtime_error("Disk device not found in /proc/diskstats");
}

float Disk::getIOUtilization(const std::string& diskName, int intervalMs) {
    try {
        long io_time_start = getDiskIOTime(diskName);

        // Wait for the specified interval
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));

        long io_time_end = getDiskIOTime(diskName);

        // Calculate the change in I/O time during the interval
        long io_time_delta = io_time_end - io_time_start;

        // Utilization is the percentage of time the disk was busy in the interval
        float utilization = (static_cast<float>(io_time_delta) / intervalMs) * 100.0f;
        return utilization;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return -1.0f; // Indicate an error
    }
}

void Disk::printStats() {
    const std::string diskName = "sda";  // Set this to your disk (e.g., "sda")
    const int intervalMs = 10;         // Interval in milliseconds (e.g., 1000 ms = 1 second)

    while (running) {
        // Clear the screen (optional, if you want to refresh output each time)
        std::cout << "\033[2J\033[1;1H";  // ANSI escape code to clear screen

        std::cout << "Disk Device: ";
        printDiskDevice();

        float utilization = getIOUtilization(diskName, intervalMs);
        if (utilization >= 0) {
            std::cout << "Disk I/O Utilization (" << diskName << "): " << utilization << "%\n";
        }
        std::cout << "Disk Active Time: " << getActiveTime() << " sec\n";
        std::cout << "Disk Capacity: " << getCapacity() << " GB\n";
        std::cout << "Used Space: " << getUsedSpace() << " GB\n";
        std::cout << "Free Space: " << getFreeSpace() << " GB\n";
        std::cout << "Response Time: " << getResponseTime() << " ms\n";

        // Wait for 1 second before the next update
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

std::string findDiskDevice() {
    for (const auto& entry : fs::directory_iterator("/dev")) {
        std::string path = entry.path().string();
        if (path.find("sd") != std::string::npos && path.length() == 8) {
            std::cout << "Found disk device: " << path << std::endl;
            return path;
        }
    }
    throw std::runtime_error("No disk device found.");
}

Disk::Disk() {
    devicePath = findDiskDevice();
}

void Disk::printDiskDevice() {
    try {
        std::cout << "Disk Device Path: " << devicePath << "\n";
        
        std::string deviceName = devicePath.substr(devicePath.find_last_of('/') + 1);

        // Model
        std::string model = readSysFile("/sys/block/" + deviceName + "/device/model");
        std::cout << "Model: " << model << "\n";

        // Serial Number
        try {
            std::string serial = readSysFile("/sys/block/" + deviceName + "/device/serial");
            std::cout << "Serial Number: " << serial << "\n";
        } catch (const std::exception &e) {
            std::cerr << "Serial Number: Not available for this device.\n";
        }

        // Disk Size in MB
        std::string sizeStr = readSysFile("/sys/block/" + deviceName + "/size");
        long long sizeInSectors = std::stoll(sizeStr);
        long long diskSizeMB = (sizeInSectors * 512) / (1024 * 1024);
        std::cout << "Size: " << diskSizeMB << " MB\n";

        // Logical Sector Size
        std::string logicalSectorSize = readSysFile("/sys/block/" + deviceName + "/queue/logical_block_size");
        std::cout << "Logical Sector Size: " << logicalSectorSize << " bytes\n";

        // Physical Sector Size
        std::string physicalSectorSize = readSysFile("/sys/block/" + deviceName + "/queue/physical_block_size");
        std::cout << "Physical Sector Size: " << physicalSectorSize << " bytes\n";

        // Device Status
        std::string status = readSysFile("/sys/block/" + deviceName + "/device/state");
        std::cout << "Device Status: " << status << "\n";
        
    } catch (const std::exception &e) {
        std::cerr << "Error retrieving disk device information: " << e.what() << "\n";
    }
}

std::string Disk::readSysFile(const std::string &path) const {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Unable to open file: " + path);
    }
    
    std::string content;
    std::getline(file, content);
    file.close();
    return content;
}

float Disk::getActiveTime() {
    try {
        std::ifstream diskStatsFile("/proc/diskstats");
        if (!diskStatsFile) {
            throw std::runtime_error("Unable to open /proc/diskstats");
        }

        std::string line;
        std::string deviceName = devicePath.substr(devicePath.find_last_of('/') + 1);

        while (std::getline(diskStatsFile, line)) {
            if (line.find(deviceName) != std::string::npos) {
                std::istringstream iss(line);
                std::string discard;
                long io_time;
                for (int i = 0; i < 12; ++i) {
                    iss >> discard;  // Skip to io_time field
                }
                iss >> io_time;

                // Convert io_time from milliseconds to seconds for easier readability
                return static_cast<float>(io_time) / 1000.0f;
            }
        }

        throw std::runtime_error("Disk device not found in /proc/diskstats");

    } catch (const std::exception &e) {
        std::cerr << "Error retrieving active time: " << e.what() << "\n";
        return 0.0f;
    }
}

// Function to measure write speed
float Disk::getWriteSpeed() {
    auto start = std::chrono::high_resolution_clock::now();
    executeCommand("dd if=/dev/zero of=testfile.bin bs=1M count=10 oflag=dsync > /dev/null 2>&1");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    float writeTime = duration.count();
    if (writeTime > 0) {
        return 10.0f / writeTime;  // 10 MB file size
    } else {
        std::cerr << "Invalid timing for write speed calculation." << std::endl;
        return 0.0f;
    }
}

// Function to measure read speed
float Disk::getReadSpeed() {
    auto start = std::chrono::high_resolution_clock::now();
    executeCommand("dd if=testfile.bin of=/dev/null bs=1M count=10 > /dev/null 2>&1");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    float readTime = duration.count();
    if (readTime > 0) {
        return 10.0f / readTime;  // 10 MB file size
    } else {
        std::cerr << "Invalid timing for read speed calculation." << std::endl;
        return 0.0f;
    }
}

float Disk::getCapacity() {
    struct statvfs stat;
    if (statvfs("/", &stat) != 0) {
        return 0.0;  // Error in retrieving filesystem stats
    }
    float capacity = static_cast<float>(stat.f_blocks) * stat.f_frsize / (1024 * 1024 * 1024);  // Bytes to GB
    return capacity;
}

float Disk::getUsedSpace() {
    struct statvfs stat;
    if (statvfs("/", &stat) != 0) {
        return 0.0;  // Error in retrieving filesystem stats
    }
    float used = (static_cast<float>(stat.f_blocks - stat.f_bfree)) * stat.f_frsize / (1024 * 1024 * 1024);  // Bytes to GB
    return used;
}

float Disk::getFreeSpace() {
    struct statvfs stat;
    if (statvfs("/", &stat) != 0) {
        return 0.0;  // Error in retrieving filesystem stats
    }
    float free = static_cast<float>(stat.f_bfree) * stat.f_frsize / (1024 * 1024 * 1024);  // Bytes to GB
    return free;
}

// Function to get response time
float Disk::getResponseTime() {
    auto start = std::chrono::high_resolution_clock::now();
    executeCommand("dd if=/dev/zero of=testfile.bin bs=4 count=1 oflag=dsync > /dev/null 2>&1");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> responseTime = end - start;
    return responseTime.count();  // Response time in milliseconds
}
