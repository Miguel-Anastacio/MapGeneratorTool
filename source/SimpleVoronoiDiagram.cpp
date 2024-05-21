#include "SimpleVoronoiDiagram.h"
#include <unordered_set>
#include <cassert>
namespace MapGeneratorTool
{
	namespace VoronoiDiagram
	{
		using namespace Utils;
		std::vector<Point> GenerateSeeds(int numberOfSeeds, int width, int height)
		{
			std::vector<Point> seeds(numberOfSeeds);
			for (int i = 0; i < numberOfSeeds; i++)
			{
				seeds[i] = Point(rand() % width, rand() % height);
			}
			return seeds;
		}

		std::unordered_map<Point, Color> GenerateColorMap(const std::vector<Point>& seeds)
		{
			std::unordered_map<Point, Color> colorMap;
			std::unordered_set<Color> colorsInUse;
			for (int i = 0; i < seeds.size(); i++)
			{
				Color color;
				do {
					color.RandColor();
				} 
				while (colorsInUse.contains(color));

				colorMap.emplace(seeds[i], color);

			}

			return colorMap;
		}

		std::vector<Point> GenerateDiagram(const std::vector<Point>& seeds, int width, int height)
		{
			std::vector<Point> diagram(width * height);
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					float minDist = std::numeric_limits<float>::max();
					Point closestSeed(-100, -100);
					for (size_t i = 0; i < seeds.size(); ++i) 
					{
						float dist = distanceSquared(Point(x, y), seeds[i]);
						if (dist < minDist) 
						{
							minDist = dist;
							closestSeed = seeds[i];
						}
					}

					diagram[width * y + x] = closestSeed;
				}
			}

			for (const auto& test : diagram)
			{
				assert(test != Point(-100, -100));
			}

			return diagram;
		}

		std::vector<Point> GenerateDiagramFromMask(const std::vector<Point>& seeds, int width, int height, const std::vector<uint8_t> mask)
		{
			std::vector<Point> diagram(width * height);
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					// point type
					int type = mask[y * width + x];
					float minDist = std::numeric_limits<float>::max();
					Point closestSeed(-100, -100);
					for (size_t i = 0; i < seeds.size(); ++i)
					{
						float dist = distanceSquared(Point(x, y), seeds[i]);

						// seed point type
						int seedType = mask[seeds[i].Y * width + seeds[i].X];

						if (dist < minDist && type == seedType)
						{
							minDist = dist;
							closestSeed = seeds[i];
						}
					}

					diagram[width * y + x] = closestSeed;
				}
			}

			for (const auto& test : diagram)
			{
				assert(test != Point(-100, -100));
			}

			return diagram;
		}


		//std::vector<
	}
}