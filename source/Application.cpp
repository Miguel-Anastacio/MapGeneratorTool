#pragma once
#include "../headers/Application.h"
#include "../lodepng/textureHandler.h"
#include "../headers/SimpleVoronoiDiagram.h"
namespace MapGeneratorTool
{
	using namespace Utils;
	void Run() 
	{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		const unsigned width = 1024, height = 1024;
		const int seedsNumber = 10;
		std::vector<uint8_t> image(width * height * 4);
		std::vector<Point> seeds = VoronoiDiagram::GenerateSeeds(seedsNumber, width, height);


		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				image[4 * width * y + 4 * x + 0] = 255;
				image[4 * width * y + 4 * x + 1] = 255;
				image[4 * width * y + 4 * x + 2] = 255;
				image[4 * width * y + 4 * x + 3] = 255;
			}
		}

		std::unordered_map<Point, Color> colorMap = VoronoiDiagram::GenerateColorMap(seeds);
		std::vector<Point> diagram = VoronoiDiagram::GenerateDiagram(seeds, width, height);

		/*for (int i = 0; i < seedsNumber; i++)
		{
			Point pt = seeds[i];
			image[4 * width * pt.Y + 4 * pt.X + 0] = colorMap[pt].R;
			image[4 * width * pt.Y + 4 * pt.X + 1] = colorMap[pt].G;
			image[4 * width * pt.Y + 4 * pt.X + 2] = colorMap[pt].B;
			image[4 * width * pt.Y + 4 * pt.X + 3] = 255;
		}*/

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				Point pt = diagram[width * y + x];
				image[4 * width * y + 4 * x + 0] = colorMap[pt].R;
				image[4 * width * y + 4 * x + 1] = colorMap[pt].G;
				image[4 * width * y + 4 * x + 2] = colorMap[pt].B;
				image[4 * width * y + 4 * x + 3] = 255;
			}
		}

		MapGeneratorTool::textureHandler::encodeOneStep("lookup.png", image, width, height);
		MapGeneratorTool::textureHandler::encodeOneStep("terrain.png", image, width, height);

	}
}