#pragma once
#include "json.hpp"
#include <fstream> 
#include <filesystem>

namespace tools
{
class FileHandler
{
public:
	bool static OutputJsonArrayToFIle(const std::filesystem::path& path, const nlohmann::ordered_json& data, int indent = 4);
};

}// namespace tools