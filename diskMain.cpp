//#include "Process.h"        // Ensure Process.h is available
#include "disk.h"
#include <iostream>
#include <filesystem>       // For std::filesystem
#include <vector>           // For std::vector
#include <algorithm>        // For std::all_of
#include <fstream>          // For std::ifstream
#include <string>
#include <thread>
#include <atomic>
#include <csignal>
namespace fs = std::filesystem;



std::atomic<bool> running(true);  // Global flag to control the loop

// Signal handler function to set the flag when a signal is received (e.g., Ctrl+C)
void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nProgram interrupted. Exiting...\n";
        running = false;  // Set flag to false to exit the loop
    }
}

int main() {
// Register signal handler for SIGINT (Ctrl+C)
  //  std::signal(SIGINT, signalHandler);
    
    std::cout << "System Resource Analyzer\n";
//    Disk analysis

    Disk disk;
    disk.printDiskDevice();
    
    std::cout << "Press Ctrl+C to stop the program.\n";
    
    // Run until signal is received
    while (running) {
        // Clear the screen (optional, if you want to refresh output each time)
        std::cout << "\033[2J\033[1;1H";  // ANSI escape code to clear screen

        // Print disk stats
        disk.printStats();

        // Wait for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Program ended.\n";
 
  /* 
    std::vector<Process> processes;
    int count = 0;

    // Loop through the /proc directory to find processes
    for (const auto& entry : fs::directory_iterator("/proc")) {
        // Each PID directory in /proc is a process
        if (entry.is_directory()) {
            std::string dirName = entry.path().filename().string();

            // Check if the directory name is a number (PID)
            if (std::all_of(dirName.begin(), dirName.end(), ::isdigit)) {
                int pid = std::stoi(dirName);

                // Retrieve the process name from /proc/[pid]/comm
                std::ifstream commFile("/proc/" + dirName + "/comm");
                std::string processName;
                if (commFile.is_open()) {
                    std::getline(commFile, processName);
                    commFile.close();

                    // Create a Process object and add to vector
                    processes.emplace_back(pid, processName);
                    count++;

                    // Stop after collecting 5 processes
                    if (count >= 5) break;
                }
            }
        }
    }

    // Display each process’s information
    for (Process& proc : processes) {
        proc.display();
    }
    */

    return 0;
}
