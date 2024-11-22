#ifndef DISK_H
#define DISK_H

#include <string>

// Forward declaration for the Disk class
class Disk {
public:
    // Constructor
    Disk();

    // Function to print general disk statistics
    void printStats();

    // Function to print disk device information
    void printDiskDevice();

    // Function to get disk's I/O utilization percentage over a given interval (in milliseconds)
    float getIOUtilization(const std::string& diskName, int intervalMs);

    // Function to get disk's active time (in seconds)
    float getActiveTime();

    // Function to measure write speed in MB/s
    float getWriteSpeed();

    // Function to measure read speed in MB/s
    float getReadSpeed();

    // Function to get total disk capacity (in GB)
    float getCapacity();

    // Function to get used space (in GB)
    float getUsedSpace();

    // Function to get free space (in GB)
    float getFreeSpace();

    // Function to get response time in milliseconds
    float getResponseTime();

private:
    // Private member to store the device path (e.g., "/dev/sda")
    std::string devicePath;

    // Function to retrieve disk I/O time from /proc/diskstats
    long getDiskIOTime(const std::string& diskName);

    // Function to read the contents of a file from the /sys filesystem
    std::string readSysFile(const std::string& path) const;

};

#endif // DISK_H

