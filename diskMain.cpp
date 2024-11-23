#include "disk.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>
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
    std::signal(SIGINT, signalHandler);

    std::cout << "System Resource Analyzer\n";

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

    return 0;
}
