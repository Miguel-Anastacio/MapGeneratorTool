#pragma once
#include <vector>
#include <cassert>
#include <string>
#include "MapComponent.h"
#include "MapSpecs.h"
#include <Rasterizer.h>
#include <TileMap.h>

namespace MapGeneratorTool
{

	using namespace Utils;
	//class Texture;
	class MapMask;
	class LookupMap : public MapComponent
	{
	public:
		LookupMap(const char* name, unsigned width, unsigned height);
		//HeightMap(const Texture& texture, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& specs);

		void RegenerateLookUp(const LookupMapData& data, MapMask* landMask,  MapMask* oceanMask);
		
		//std::vector<uint8_t> GenerateLookupMapFromMask(const LookupFeatures& data, const MapMask* mask, const char* name = "example.png");
		TileMap GenerateTileMapFromMask(const LookupFeatures& data, const MapMask* mask, TileType type, const char* name = "example.png");

		Utils::Color FindClosestTileOfSameType(const std::vector<Tile>& tileMap, int x, int y, unsigned width, unsigned height) const;

		void OutputLookupTable() const ; 

	private:
		//std::vector<double> CreateHeightMap(const NoiseMapData& data) const;
		//std::vector<sf::Uint8> CreateBuffer(const std::vector<double>& data) const;

		//mygal::Diagram<double> m_diagram;
		std::unordered_set<Utils::Color> m_colorsInUse;
		LookupFeatures m_lastLookup;
		std::vector<mygal::Vector2<double>> m_centroids;

		std::unique_ptr<TileMap> m_lookUpTileMap;

	};


}