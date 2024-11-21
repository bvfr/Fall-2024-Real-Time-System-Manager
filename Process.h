#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

class Process {
public:
    Process(int pid);

    // Display process information
    void display() const;

    // Accessor methods
    int getPid() const;
    std::string getName() const;
    float getCpuUsage() const;
    float getMemoryUsage() const;
    float getDiskUsage() const;
    float getNetworkUsage() const;

    // Methods to update resource usage
    void updateUsage();

private:
    int pid;
    std::string name;
    
    // Resource usage metrics
    float cpuUsage;
    float memoryUsage;
    float diskUsage;
    float networkUsage;

    // Path to the stat file for the process
    std::string statFilePath;

    // Private helper methods to retrieve data
    std::string fetchProcessName() const;
    float fetchCpuUsage() const;
    float fetchMemoryUsage() const;
    float fetchDiskUsage() const;
    float fetchNetworkUsage() const;

    // Methods to fetch total system usage
    float fetchTotalMemory() const;
    float fetchTotalDisk() const;
    float fetchTotalNetwork() const;
};

#endif // PROCESS_H
