#pragma once
#include <iostream>
namespace MapGeneratorTool
{
namespace Logger
{
	inline void TimeMsec(const char* message, double time)
	{
		std::cout << message << " " << time << " msec" << "\n";
	}
}
}
