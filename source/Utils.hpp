#ifndef UTILSHPP
#define UTILSHPP

#include <Windows.h>
#include <filesystem>
#include <string>
#include <iostream>

namespace fs = std::filesystem;

namespace Utils
{
	inline std::string GetWorkingDirectory()
	{
		CHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");
		return std::string(buffer).substr(0, pos);
	}

	inline std::string GetResourceDirectory() 
	{
		std::string workingDirectory = GetWorkingDirectory();
		int count = 0;
		int pos = 0;
		for (int i = workingDirectory.size() - 1; i >= 0 ; i--)
		{
			if (workingDirectory[i] == '\\')
				count++;

			if (count == 3)
			{
				pos = i;
				break;
			}
				
		}

		return workingDirectory.substr(0, pos) + std::string("\\Resources\\");
	}
}

#endif