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
		Map(unsigned width, unsigned height);
		~Map();

		inline const sf::RenderTexture& LookupTexture() const
		{
			if(m_lookupmap)
				return m_lookupmap->Texture();

			return sf::RenderTexture();
		}
		inline const sf::RenderTexture& HeightMapTexture() const
		{
			if (m_heightmap)
				return m_heightmap->Texture();

			return sf::RenderTexture();
		}
		inline const sf::RenderTexture& TerrainMapTexture() const
		{
			if (m_terrainmap)
				return m_terrainmap->Texture();

			return sf::RenderTexture();
		}
		inline const sf::RenderTexture& MapMaskTexture () const
		{
			if (m_maskmap)
				return m_maskmap->Texture();

			return sf::RenderTexture();
		}
		inline const sf::RenderTexture& OceanMapMaskTexture() const
		{
			if (m_oceanMask)
				return m_oceanMask->Texture();

			return sf::RenderTexture();
		}
		inline const sf::RenderTexture& LandMapMaskTexture() const
		{
			if (m_landMask)
				return m_landMask->Texture();

			return sf::RenderTexture();
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

		void GenerateMap(const std::vector<uint8_t>& textureBuffer, unsigned width, unsigned height); 

		void GenerateMap(const std::vector<uint8_t>& textureBuffer, float cutOffHeight);
		
		//void SaveMap(const char* filePath);
		void SaveMap(const std::string& filePath) const;
		void Reset();

	private:
		//void SaveMapComponent(MapComponent* component, const char* filePath, const char* message = "map component");
		void SaveMapComponent(MapComponent* component, const std::string& filePath, const char* message = "map component") const;
		void ClearMapComponent(MapComponent* component, const char* message = "map component");
		
		std::vector<TerrainType> m_terrainTypes; 

		std::unique_ptr<LookupMap> m_lookupmap;
		std::unique_ptr<HeightMap> m_heightmap;
		std::unique_ptr<TerrainMap> m_terrainmap;
		std::unique_ptr<MapMask> m_maskmap;
		std::unique_ptr<MapMask> m_landMask;
		std::unique_ptr<MapMask> m_oceanMask;

		float m_cutOffHeight = 0.001f;
		//std::unique_ptr<Texture> m_lookUpTexture;
	};

}