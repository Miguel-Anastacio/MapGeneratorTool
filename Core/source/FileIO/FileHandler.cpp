#pragma once
#include "FileHandler.h"

namespace Core
{
	bool FileHandler::OutputJsonArrayToFIle(const std::filesystem::path& path, const nlohmann::ordered_json& data, int indent)
	{
		// Write the JSON array to a file
		std::ofstream outFile(path); // Open file in write mode
		if (outFile.is_open()) 
		{
			outFile << data.dump(indent);  // Write to file with indentation
			outFile.close();  // Close the file after writing
			return true;
		}
		else 
		{
			//std::cerr << "Could not open the file for writing!" << std::endl;
			return false;
		}
	}

}// namespace tools