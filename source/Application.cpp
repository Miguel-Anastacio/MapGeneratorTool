#pragma once
#include "Application.h"
#include "../thirdparty/lodepng/textureHandler.h"
#include "PerlinNoise.hpp"
#include "SimpleVoronoiDiagram.h"
#include "HeightMap.h"
#include "texture/Texture.h"
namespace MapGeneratorTool
{
	using namespace Utils;
	void Run() 
	{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		const unsigned width = 1024, height = 1024;
		const int seedsNumber = 10;
		std::vector<uint8_t> image(width * height * 4);
		//std::vector<Point> seeds = VoronoiDiagram::GenerateSeeds(seedsNumber, width, height);


		////for (int y = 0; y < height; y++)
		////{
		////	for (int x = 0; x < width; x++)
		////	{
		////		image[4 * width * y + 4 * x + 0] = 255;
		////		image[4 * width * y + 4 * x + 1] = 255;
		////		image[4 * width * y + 4 * x + 2] = 255;
		////		image[4 * width * y + 4 * x + 3] = 255;
		////	}
		////}

		//std::unordered_map<Point, Color> colorMap = VoronoiDiagram::GenerateColorMap(seeds);
		//std::vector<Point> diagram = VoronoiDiagram::GenerateDiagram(seeds, width, height);

		///*for (int i = 0; i < seedsNumber; i++)
		//{
		//	Point pt = seeds[i];
		//	image[4 * width * pt.Y + 4 * pt.X + 0] = colorMap[pt].R;
		//	image[4 * width * pt.Y + 4 * pt.X + 1] = colorMap[pt].G;
		//	image[4 * width * pt.Y + 4 * pt.X + 2] = colorMap[pt].B;
		//	image[4 * width * pt.Y + 4 * pt.X + 3] = 255;
		//}*/

		//for (int y = 0; y < height; y++)
		//{
		//	for (int x = 0; x < width; x++)
		//	{
		//		Point pt = diagram[width * y + x];
		//		image[4 * width * y + 4 * x + 0] = colorMap[pt].R;
		//		image[4 * width * y + 4 * x + 1] = colorMap[pt].G;
		//		image[4 * width * y + 4 * x + 2] = colorMap[pt].B;
		//		image[4 * width * y + 4 * x + 3] = 255;
		//	}
		//}

		//MapGeneratorTool::textureHandler::encodeOneStep("lookup.png", image, width, height);
		//MapGeneratorTool::textureHandler::encodeOneStep("terrain.png", image, width, height);

		const siv::PerlinNoise::seed_type seed = 127856u;

		const siv::PerlinNoise perlin{ seed };

		HeightMap map = HeightMap(width, height, "TestHM.png", 24, perlin);
		//MapGeneratorTool::textureHandler::encodeOneStep("heightMap.png", image, width, height); 
		map.SaveHeightMapToFile();
	}


	void PerlinExample()
	{

		const siv::PerlinNoise::seed_type seed = 123456u;

		const siv::PerlinNoise perlin{ seed };
		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 5; ++x)
			{
				const double noise = perlin.octave2D_01((x * 0.01), (y * 0.01), 4);

				std::cout << noise << '\t';

			}

			std::cout << '\n';
		}
	}
}