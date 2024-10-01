#pragma once
#include <vector>
#include <cassert>
#include <string>
#include "MapComponent.h"
#include "MapSpecs.h"

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
		std::vector<uint8_t> GenerateLookupMapFromMask(const LookupFeatures& data, const MapMask* mask, const char* name = "example.png");

		void OutputLookupTable() const ; 

	private:
		//std::vector<double> CreateHeightMap(const NoiseMapData& data) const;
		//std::vector<sf::Uint8> CreateBuffer(const std::vector<double>& data) const;

		//mygal::Diagram<double> m_diagram;
		std::unordered_set<Utils::Color> m_colorsInUse;

	};


}