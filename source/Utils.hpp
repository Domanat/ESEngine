#ifndef UTILSHPP
#define UTILSHPP

#include <Windows.h>
#include <string>

inline std::string GetWorkingDirectory()
{
    CHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

#endif