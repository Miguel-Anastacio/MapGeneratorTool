#pragma once
#include <vector>
#include <unordered_map>
#include "Utils.h"
namespace MapGeneratorTool
{
	namespace VoronoiDiagram
	{
		using namespace Utils;

		std::vector<Utils::Point> GenerateSeeds(int numberOfSeeds, int width, int height);

		std::unordered_map<Point, Color> GenerateColorMap(const std::vector<Point>& seeds);

		std::vector<Point> GenerateDiagram(const std::vector<Point>& seeds, int width, int height);
	}
}