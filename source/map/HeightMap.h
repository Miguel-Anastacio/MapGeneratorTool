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

	class NoiseSpecs
	{
	public:
		NoiseSpecs(double lacunarity, double persistance, int octaves) : m_lacunarity(lacunarity), m_persistance(persistance), m_octaves(octaves) {};
		NoiseSpecs() : m_lacunarity(0), m_persistance(0), m_octaves(0) {};
		NoiseSpecs(const NoiseSpecs& other)
			: m_lacunarity(other.m_lacunarity), m_persistance(other.m_persistance), m_octaves(other.m_octaves)
		{
			
		}
		~NoiseSpecs() {};
		NoiseSpecs& operator =(const NoiseSpecs& other)
		{
			if (this == &other) return *this;

			m_lacunarity = other.m_lacunarity;
			m_persistance = other.m_persistance;
			m_octaves = other.m_octaves;

			return *this;
		}
		//NoiseSpecs& operator =(const NoiseSpecs& other);

	private:
		double m_lacunarity;
		double m_persistance;
		int m_octaves;


		friend class HeightMap;
	};

	class HeightMap
	{
	public:
		HeightMap(unsigned width, unsigned height, const char* name, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& specs);
		HeightMap(const Texture& texture, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& specs);
		~HeightMap();

		void SaveHeightMapToFile();
		void RegenerateHeightMap(double noiseScale, const NoiseSpecs& specs);
	private:
		std::vector<double> CreateHeightMap(unsigned width, unsigned height, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& m_noiseSpecs);

		Texture m_texture;
		//unsigned m_width;
		//unsigned m_height;
		std::vector<double> m_noiseValues;
		double m_noiseScale;
		siv::PerlinNoise m_noise;
		NoiseSpecs m_noiseSpecs;

	};


}