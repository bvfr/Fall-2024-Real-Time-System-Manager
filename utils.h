#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string readFile(const std::string &path);
std::string executeCommand(const std::string &command);
float safe_stof(const std::string& str);

#endif

