#pragma once
#include <vector>
#include "MapComponent.h"
#include <cassert>
#include "MapSpecs.h"
namespace MapGeneratorTool
{
	struct TerrainType
	{
		double minHeight;
		sf::Color color;
		const char* name;

		TerrainType(double height = 0.0f, sf::Color col = sf::Color::Black, const char* nam = "default") : minHeight(height), color(col), name(nam) {};
	};

	using namespace Utils;
	//class Texture;
	class TerrainMap : public MapComponent
	{
	public:
		TerrainMap(const char* name, const std::vector<double>& noiseValues, unsigned width, unsigned height);
		//HeightMap(const Texture& texture, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& specs);

		void RegenerateTerrain(const std::vector<double>& noiseValues);

	private:
		//std::vector<double> CreateHeightMap(const NoiseMapData& data) const;
		std::vector<sf::Uint8> CreateBuffer(const std::vector<double>& data) const;
		sf::Color GetColorFromHeight(const std::vector<TerrainType>& colors, double height) const;

		std::vector<TerrainType> m_terrainTypes;


	};


}