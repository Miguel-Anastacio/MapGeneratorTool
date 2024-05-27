#pragma once
#include <vector>
#include "../Utils.h"
#include "../texture/Texture.h"
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
	class HeightMap : public Dimensions
	{
	public:
		HeightMap(const char* name, const NoiseMapData& data);
		//HeightMap(const Texture& texture, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& specs);
		~HeightMap();

		void SaveHeightMapToFile();
		void SaveHeightMapToFile(const char* filename);
		void RegenerateHeightMap(const NoiseMapData& data);

		inline const sf::RenderTexture& Texture() const
		{
			return m_texture;
		}

	private:
		std::vector<double> CreateHeightMap(const NoiseMapData& data) const;

		sf::RenderTexture m_texture;
		std::vector<double> m_noiseValues;
		//double m_noiseScale;
		//FastNoiseLite m_noise;
		//NoiseSpecs m_noiseSpecs;

	};


}