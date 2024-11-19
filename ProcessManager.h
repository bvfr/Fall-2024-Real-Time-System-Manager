#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "Process.h"
#include <vector>
#include <string>

class ProcessManager {
public:
    ProcessManager();
    const std::vector<Process>& getProcesses() const;

    void loadProcesses();
    void refreshProcesses();
    void displayProcesses() const;
    void sortProcesses(const std::string& criteria);
    void showNextPage();
    void showPreviousPage();

private:
    std::vector<Process> processes;
    size_t currentPage;
    const size_t pageSize = 10;

    // Helper methods
    void sortByCpuUsage();
    void sortByMemoryUsage();
    void sortByDiskUsage();
    void sortByNetworkUsage();
};

#endif // PROCESS_MANAGER_H

