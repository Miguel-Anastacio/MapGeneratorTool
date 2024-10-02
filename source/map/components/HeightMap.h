#pragma once
#include <vector>
#include "MapComponent.h"
#include <cassert>
#include <algorithm>
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include "MapSpecs.h"
class FastNoiseLite;
namespace MapGeneratorTool
{
	using namespace Utils;
	//class Texture;
	class HeightMap : public MapComponent
	{
	public:
		HeightMap(const char* name, const NoiseMapData& data);
		HeightMap(const char* name, unsigned width, unsigned height, std::vector<double>&& elevation);
		//HeightMap(const Texture& texture, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& specs);
		~HeightMap();

		void RegenerateHeightMap(const NoiseMapData& data);
		inline std::vector<double> NoiseMap() const 
		{
			return m_elevation;
		}

		void SetNoiseMap(std::vector<double>&& elevation);

	private:
		std::vector<double> CreateHeightMap(const NoiseMapData& data) const;
		std::vector<sf::Uint8> CreateBuffer();

		std::vector<double> m_elevation;

		//double m_noiseScale;
		//FastNoiseLite m_noise;
		//NoiseSpecs m_noiseSpecs;

	};


}