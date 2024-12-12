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
#include "sqlite3.h"
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


class StringList {
    private:
        std::vector<std::string> list;

    public:
        // Function to check if a string is in the list
        bool contains(const std::string& str) const {
            return std::find(list.begin(), list.end(), str) != list.end();
        }

        // Function to add a string to the list
        void add(const std::string& str) {
            if (!contains(str)) { // Only add if the string is not already in the list
                list.push_back(str);
            } else {
                std::cout << "String \"" << str << "\" is already in the list.\n";
            }
        }

        // Function to print all strings in the list
        void print() const {
            for (const auto& str : list) {
                std::cout << str << "\n";
            }
        }
};