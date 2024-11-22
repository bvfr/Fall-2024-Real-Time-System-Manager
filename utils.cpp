#include "utils.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>

std::string readFile(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string executeCommand(const std::string &command) {
    char buffer[128];
    std::string result = "";
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) return "Error";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

// Helper function to safely convert a string to float
float safe_stof(const std::string& str) {
    try {
        return std::stof(str);
    } catch (...) {
        std::cerr << "Conversion error for string: " << str << std::endl;
        return 0.0f;
    }
}

