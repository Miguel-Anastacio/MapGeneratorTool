#pragma once
#include <vector>
#include <unordered_map>
#include "Utils.h"
#include "utils/Point.h"
#include "utils/Color.h"
#include "texture/Texture.h"
#include "MyGAL/Diagram.h"
namespace MapGeneratorTool
{
	namespace SimpleVoronoiDiagram
	{
	using namespace Utils;
	using ::std::vector;
	using ::std::unordered_map;

	vector<Utils::Point> GenerateSeeds(int numberOfSeeds, int width, int height);
	unordered_map<Utils::Point, Utils::Color> GenerateColorMap(const vector<Point>& seeds);

	vector<Point> GenerateDiagram(const vector<Point>& seeds, int width, int height);

	vector<Point> GenerateDiagramFromMask(const vector<Point>& seeds, int width, int height, const vector<uint8_t> mask);

	vector<Point> ComputeCentroids(const vector<Point>& diagram, int width, int height, const vector<Point>& seeds);

	template <typename T>
	void WriteDiagramToTexture(const Texture& texture, mygal::Diagram<T>& diagram);

	template<typename T>
	void WriteDiagramToTexture(const Texture& texture, mygal::Diagram<T>& diagram)
	{

	}

	}

	
}