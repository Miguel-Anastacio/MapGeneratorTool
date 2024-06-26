#pragma once
#include <memory>
#include "../Utils.h"
#include"../utils/Point.h"
#include"../utils/Color.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <MyGAL/Diagram.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include "MapSpecs.h"
#include "HeightMap.h"
#include "TerrainMap.h"
namespace MapGeneratorTool
{
	class Texture;
	using namespace Utils;
	class Map : public Dimensions
	{
	public:
		//Map(const char* maskFileName, int seeds, const char* lookUpTextureName,int iterLloyd);
		Map(unsigned width, unsigned height, int seeds, const char* lookUpTextureName, int iterLloyd=0);
		~Map();

		inline const sf::RenderTexture& lookupTexture() const
		{
			return m_lookupTexture;
		}
		inline const sf::RenderTexture& HeightMapTexture() const
		{
			return m_heightmap->Texture();
		}
		inline const sf::RenderTexture& TerrainMapTexture() const
		{
			return m_terrainmap->Texture();
		}

		inline const std::vector<double> NoiseMap() const
		{
			assert(m_heightmap != nullptr);
			return m_heightmap->NoiseMap();
		}

		inline void AddTerrainType(const TerrainType& type)
		{
			m_terrainTypes.push_back(type);
		}

		inline const std::vector<TerrainType>& TerrainTypes() const
		{
			return m_terrainTypes;
		}

		void GenerateMap(const LookupMapData& data);
		void GenerateHeightMap(const NoiseMapData& data);

		void GenerateTerrainMap(const std::vector<double>& noiseMap);
		void GenerateTerrainMap(const std::vector<double>& noiseMap, const std::vector<TerrainType>& types);

		void SaveLookupMapToFile() const;
		void SaveLookupMapToFile(const char* filename) const;

		void SaveHeightMapToFile(const char* filename) const;

	private:
		inline int valSeeds(int seeds)
		{
			return std::clamp(seeds, 0, static_cast<int>(std::pow(256, 3)));
		}

		// not in use
		void CreateLookUpTextureFromMask(const Texture& mask);
		void PopulateTexture(const std::unordered_map<Point, Color>& colorMap, const std::vector<Point>& diagram, Texture* texture) const;
		std::vector<uint8_t> GenerateMaskData(const Texture& mask) const;
		void OutputSeedPoints(const std::vector<Point>& seeds) const;
		int m_divisions;
		////////////////////////////
		

		const char* m_lookupTextureName;
		mygal::Diagram<double> m_diagram;
		sf::RenderTexture m_lookupTexture;
		

		std::unique_ptr<HeightMap> m_heightmap;
		std::unique_ptr<TerrainMap> m_terrainmap;

		std::vector<TerrainType> m_terrainTypes;

		//std::unique_ptr<Texture> m_lookUpTexture;
	};

}