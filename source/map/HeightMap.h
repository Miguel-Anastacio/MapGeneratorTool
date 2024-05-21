#pragma once
#include <vector>
#include "../Utils.h"
#include "../texture/Texture.h"
#include <cassert>
#include <algorithm>
#include <memory>
class FastNoiseLite;
namespace MapGeneratorTool
{
	using namespace Utils;
	//class Texture;
	class NoiseSpecs
	{
	public:
		NoiseSpecs(double lacunarity, double persistance, int octaves) : 
			m_lacunarity(valLacunarity(lacunarity)),
			m_persistance(valPersistance(persistance)), 
			m_octaves(valOctaves(octaves)) {};

		NoiseSpecs() : m_lacunarity(0), m_persistance(0), m_octaves(0) {};

		NoiseSpecs(const NoiseSpecs& other)
			: m_lacunarity(other.m_lacunarity), 
			m_persistance(other.m_persistance), 
			m_octaves(other.m_octaves)
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


		inline double lacunarity() const{ 
			return m_lacunarity;
		}

		inline double persistance() const {
			return m_persistance;
		}
		inline int octaves() const {
			return m_octaves;
		}

	private:

		inline double valLacunarity(double lacunarity) const
		{
			if (lacunarity < 1.0)
				lacunarity = 1.0;
			return lacunarity;

		}

		inline double valPersistance(double persistance) const
		{
			return std::clamp(persistance, 0.0, 1.0);
		}

		inline int valOctaves(int octaves) const
		{
			if (octaves < 0)
				octaves = 0;
			return octaves;
		}


		double m_lacunarity;
		double m_persistance;
		int m_octaves;


		//friend class HeightMap;
	};

	class HeightMap
	{
	public:
		HeightMap(unsigned width, unsigned height, const char* name, double noiseScale, const FastNoiseLite& noise, const NoiseSpecs& specs);
		//HeightMap(const Texture& texture, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& specs);
		~HeightMap();

		void SaveHeightMapToFile();
		void RegenerateHeightMap(double noiseScale, const FastNoiseLite& noise, const NoiseSpecs& specs);
	private:
		std::vector<double> CreateHeightMap(unsigned width, unsigned height, double noiseScale, const FastNoiseLite& noise, const NoiseSpecs& m_noiseSpecs);

		std::unique_ptr<Texture> m_texture;
		std::vector<double> m_noiseValues;
		//double m_noiseScale;
		//FastNoiseLite m_noise;
		//NoiseSpecs m_noiseSpecs;

	};


}