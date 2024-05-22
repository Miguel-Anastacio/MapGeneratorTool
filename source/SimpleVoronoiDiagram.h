#pragma once
#include <vector>
#include <unordered_map>
#include "Utils.h"
#include "utils/Point.h"
namespace MapGeneratorTool
{
	namespace VoronoiDiagram
	{
		using namespace Utils;

		std::vector<Utils::Point> GenerateSeeds(int numberOfSeeds, int width, int height);

		std::unordered_map<Point, Color> GenerateColorMap(const std::vector<Point>& seeds);

		std::vector<Point> GenerateDiagram(const std::vector<Point>& seeds, int width, int height);
		std::vector<Point> GenerateDiagramFromMask(const std::vector<Point>& seeds, int width, int height, const std::vector<uint8_t> mask);

		std::vector<Point> ComputeCentroids(const std::vector<Point>& diagram, int width, int height, const std::vector<Point>& seeds);
	}
}