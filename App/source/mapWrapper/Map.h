#pragma once
#include <memory>
#include "data/Utils.h"
#include "data/Color.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <MyGAL/Diagram.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include "map/MapSpecs.h"
#include "map/Map.h"
namespace MapGeneratorTool
{
	class Texture;
	using namespace MapGenerator::data;
	using namespace MapGenerator;
	class MapSFML : public Dimensions
	{
	public:
		MapSFML(unsigned width, unsigned height);
		~MapSFML();

		inline const sf::RenderTexture* LookupTexture() const
		{
			return &m_lookupTexture;
		}
		inline const sf::RenderTexture* HeightMapTexture() const
		{
			return &m_heighMapTexture;
		}
		inline const sf::RenderTexture* TerrainMapTexture() const
		{
			return &m_terrainTexture;
		}
		inline const sf::RenderTexture* MapMaskTexture () const
		{
			return &m_maskTexture;
		}
		inline const sf::RenderTexture* OceanMapMaskTexture() const
		{
			return &m_lookupTextureOcean;
		}
		inline const sf::RenderTexture* LandMapMaskTexture() const
		{
			return &m_lookupTextureLand;
		}

		inline MapGenerator::Map& GetMap() 
		{
			return m_map;
		}

		//inline const std::vector<double> NoiseMap() const
		//{
		//	/*assert(m_heightmap != nullptr);
		//	return m_heightmap->NoiseMap();*/
		//}

		inline void AddTerrainType(const TerrainType& type)
		{
			m_terrainTypes.push_back(type);
		}

		inline const std::vector<TerrainType>& TerrainTypes() const
		{
			return m_terrainTypes;
		}

		void GenerateHeightMap(const NoiseMapData& data);
		void GenerateHeightMapTectonic();
		void GenerateHeightMapTectonic(const NoiseMapData& data);

		void GenerateTerrainMap(const std::vector<double>& noiseMap);
		void GenerateTerrainMap(const std::vector<double>& noiseMap, const std::vector<TerrainType>& types);


		void RegenerateLookUp(const LookupMapData& data);
		void RegenerateLookupBorders(const LookupMapData& data);

		void GenerateMap(const std::vector<uint8_t>& textureBuffer, unsigned width, unsigned height); 

		void GenerateMapFromHeigthMap(const std::vector<uint8_t>& textureBuffer, float cutOffHeight);
		
		void SaveMap(const std::string& filePath) const;
		void Reset();



	private:
		//void SaveMapComponent(MapComponent* component, const char* filePath, const char* message = "map component");
		//void SaveMapComponent(MapComponentSFML* component, const std::string& filePath, const char* message = "map component") const;
		//void ClearMapComponent(MapComponentSFML* component, const char* message = "map component");x

		void OutputLookupTable() const;
		
		std::vector<TerrainType> m_terrainTypes; 

		MapGenerator::Map m_map;

		sf::RenderTexture m_lookupTexture;
		sf::RenderTexture m_lookupTextureLand;
		sf::RenderTexture m_lookupTextureOcean;
		sf::RenderTexture m_maskTexture;
		sf::RenderTexture m_heighMapTexture;
		sf::RenderTexture m_terrainTexture;
	};

}