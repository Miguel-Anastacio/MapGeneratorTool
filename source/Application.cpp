#pragma once
#include "Application.h"
#include "../thirdparty/lodepng/textureHandler.h"
#include "../thirdparty/fastNoiseLite/FastNoiseLite.h"
#include "SimpleVoronoiDiagram.h"
#include "HeightMap.h"
#include "texture/Texture.h"
#include "../map/Map.h"
#include <SFML/Graphics.hpp>
namespace MapGeneratorTool
{
	using namespace Utils;







	void Run() 
	{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		const unsigned width = 1024, height = 1024;
		const int seedsNumber = 400;
		

		//FastNoiseLite noise;
		//noise.SetSeed(100123);
		//noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		//noise.SetFrequency(0.01f);
		//noise.SetFractalOctaves(5);
		//noise.SetFractalType(FastNoiseLite::FractalType_FBm);
		//noise.SetFractalLacunarity(2.0f);
		//noise.SetFractalGain(0.5f);
		//HeightMap map1 = HeightMap(width, height, "heightMap1.png", 1, noise, NoiseSpecs(1, 1, 1));
		//HeightMap map = HeightMap(width, height, "TestHM- Lacunarity2.png", 302.46, perlin, NoiseSpecs(1, 1, 1));
		//HeightMap map2 = HeightMap(width, height, "TestHM3-Lacunarity16.png", 27.6, perlin, NoiseSpecs(16, 0.5, 8));
		//HeightMap map3 = HeightMap(width, height, "TestHM4Lacunarity32.png", 27.6, perlin, NoiseSpecs(32, 0.5, 3));
		//MapGeneratorTool::textureHandler::encodeOneStep("heightMap.png", image, width, height); 
		//map1.SaveHeightMapToFile();s
		//map1.SaveHeightMapToFile();
		//map2.SaveHeightMapToFile();
		//map3.SaveHeightMapToFile();

		//Map newMap = Map(width, height, seedsNumber, "lookup3lloyd.png");
		//std::vector<Vector2> points = FortuneAlgo::generatePoints(seedsNumber);
		//VoronoiDiagram diagram = FortuneAlgo::generateRandomDiagram(seedsNumber);
		//FortuneAlgo::drawDiagram(diagram);
		//Map mapWithMask = Map("mask.png", seedsNumber, "lookup3mask.png");


		auto points = generatePoints<double>(1000);
		mygal::Diagram diagram = generateDiagram(points);
		auto polygons = diagram.GetPolygons();
		drawPolygons(polygons);



	}


}