#ifndef DASH_UTILS_H
#define DASH_UTILS_H

#include <string>

void Log(std::string message, const char* funcname = __builtin_FUNCTION());
std::string GetAbsoloutePathFromRelative(const std::string &relativePath);

#endif //DASH_UTILS_H
