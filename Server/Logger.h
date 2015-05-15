#pragma once

#include <iostream>
#include <string>
#include <cstdarg>

#define LOG_PRINT(message, ...) printf(message, ##__VA_ARGS__)

class Logger
{
public:
	template<class... Args>
	static void Print(const char* format, Args&&... args)
	{
		std::string buff(format);
		buff.append("\n");
	
		printf(buff.c_str(), std::forward<Args>(args)...);
	}
};


