#pragma once
#include <memory>
#include "Utils.h"
#include "Point.h"
#include "Color.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <MyGAL/Diagram.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include "MapSpecs.h"
#include "components/HeightMap.h"
#include "components/TerrainMap.h"
#include "components/MapMask.h"
#include "components/LookupMap.h"
namespace MapGeneratorTool
{
	class Texture;
	using namespace Utils;
	class Map : public Dimensions
	{
	public:
		Map(unsigned width, unsigned height, int seeds, const char* lookUpTextureName, int iterLloyd=0);
		~Map();

		inline const sf::RenderTexture& lookupTexture() const
		{
			if(m_lookupmap)
				return m_lookupmap->Texture();

			return sf::RenderTexture();
		}
		inline const sf::RenderTexture& HeightMapTexture() const
		{
			return m_heightmap->Texture();
		}
		inline const sf::RenderTexture& TerrainMapTexture() const
		{
			return m_terrainmap->Texture();
		}
		inline const sf::RenderTexture& MapMaskTexture () const
		{
			return m_maskmap->Texture();
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

		void GenerateHeightMap(const NoiseMapData& data);

		void GenerateTerrainMap(const std::vector<double>& noiseMap);
		void GenerateTerrainMap(const std::vector<double>& noiseMap, const std::vector<TerrainType>& types);

		//void SaveLookupMapToFile() const;
		//void SaveLookupMapToFile(const char* filename) const;
		//void SaveHeightMapToFile(const char* filename) const;


		void RegenerateLookUp(const LookupMapData& data);

		void GenerateMaskFromHeightMapTexture(const std::vector<uint8_t>& textureBuffer, float cutOffHeight); 

		void GenerateMap(const std::vector<uint8_t>& textureBuffer, float cutOffHeight);
		
		void SaveMap();
		//void ClearAll();

	private:
		void SaveMapComponent(MapComponent* component);
		inline int valSeeds(int seeds) const
		{
			return std::clamp(seeds, 0, static_cast<int>(std::pow(256, 3)));
		}

		void CreateLookUpTextureFromMask(const Texture& mask);
		
		const char* m_lookupTextureName;


		std::unique_ptr<LookupMap> m_lookupmap;
		std::unique_ptr<HeightMap> m_heightmap;
		std::unique_ptr<TerrainMap> m_terrainmap;
		std::unique_ptr<MapMask> m_maskmap;
		std::unique_ptr<MapMask> m_landMask;
		std::unique_ptr<MapMask> m_oceanMask;

		std::vector<TerrainType> m_terrainTypes;

		//std::unique_ptr<Texture> m_lookUpTexture;
	};

}