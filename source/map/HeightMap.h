#pragma once
#include <vector>
#include "..\Utils.h"
#include "../texture/Texture.h"
#include "../PerlinNoise.hpp"
#include <cassert>
namespace MapGeneratorTool
{
	using namespace Utils;
	//class Texture;

	class HeightMap
	{
	public:
		HeightMap(unsigned width, unsigned height, const char* name, double noiseScale, const siv::PerlinNoise& noise);
		~HeightMap();

		void SaveHeightMapToFile();

	private:
		std::vector<double> CreateHeightMap(unsigned width, unsigned height, double noiseScale, const siv::PerlinNoise& noise);

		Texture m_texture;
		//unsigned m_width;
		//unsigned m_height;
		std::vector<double> m_noiseValues;
		double m_noiseScale;
		siv::PerlinNoise m_noise;
		double m_lacunarity;
		double m_persistance;
		int m_octaves

	};


}