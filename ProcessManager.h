#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "Process.h"
#include <vector>
#include <string>

class ProcessManager {
public:
    ProcessManager();
    
    // Get all processes
    const std::vector<Process>& getProcesses() const;
    
    // Process management methods
    void loadProcesses();
    void sortProcesses(const std::string& criteria);

private:
    std::vector<Process> processes;
    size_t currentPage;
    static const size_t pageSize = 10;
};

#endif // PROCESS_MANAGER_H
