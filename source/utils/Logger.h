#pragma once
#include <iostream>
namespace MapGeneratorTool
{
namespace Logger
{
	enum class Type
	{
		INFO = 0,
		WARNING = 1,
		ERROR = 2
	};


	inline void TimeMsec(const char* message, double time)
	{
		std::cout << message << " " << time << " msec" << "\n";
	}

	template<typename T>
	void LogObject(Type logType, const char* message, const T& object)
	{
		std::string type;
		switch (logType)
		{
		case MapGeneratorTool::Logger::Type::INFO:
			type = "INFO: ";
			break;
		case MapGeneratorTool::Logger::Type::WARNING:
			type = "WARNING: ";
			break;
		case MapGeneratorTool::Logger::Type::ERROR:
			type = "ERROR: ";
			break;
		default:
			break;
		}

		std::cout << type << message << object << "\n";
	}
}
}
