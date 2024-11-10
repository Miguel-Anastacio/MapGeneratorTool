#pragma once
#include <vector>
#include "MapComponent.h"
#include <cassert>
#include <algorithm>
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include "map/MapSpecs.h"
class FastNoiseLite;
namespace MapGeneratorTool
{
	class HeightMapWrapper : public MapComponentSFML
	{
	public:
		HeightMapWrapper(const char* name, const NoiseMapData& data);
		HeightMapWrapper(const char* name, unsigned width, unsigned height, std::vector<double>&& elevation);
		//HeightMap(const Texture& texture, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& specs);
		~HeightMapWrapper();

		void RegenerateHeightMap(const NoiseMapData& data);
		inline std::vector<double> NoiseMap() const 
		{
			return m_mapData.;
		}

		void SetNoiseMap(std::vector<double>&& elevation);

	private:
		std::vector<double> CreateHeightMapFromNoise(const NoiseMapData& data) const;
		void CreateHeightTectonicPlates() ;
		void CreateHeightTectonicPlates(const NoiseMapData& data);
		std::vector<sf::Uint8> CreateBuffer();


		//double m_noiseScale;
		//FastNoiseLite m_noise;
		//NoiseSpecs m_noiseSpecs;

	};


}