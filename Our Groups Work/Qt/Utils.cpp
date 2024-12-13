//
// Created by Jake on 10/12/2024.
//

#include "DatasetInterface.h"
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include "Utils.h"
#include <filesystem>

// Debugging log utility function
void Log(std::string message, const char* funcname)
{
    auto start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string currentTime = std::ctime(&start); // Convert time to string

    // Remove trailing newline from ctime output
    currentTime.erase(currentTime.find_last_not_of("\n") + 1);

    std::cout << "[" << funcname << " (" << currentTime << ")] " << message << std::endl;
}

std::string GetAbsoloutePathFromRelative(const std::string &relativePath) {
    try {
        std::filesystem::path path(relativePath);
        return std::filesystem::absolute(path).string();
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Error resolving absolute path: " << e.what() << std::endl;
        return "";
    }
}