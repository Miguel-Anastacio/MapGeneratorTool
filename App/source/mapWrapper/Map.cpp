#include "mapwrapper/Map.h"
#include "graphics/Renderer.h"
#include "map/TileMap.h"
namespace MapGeneratorTool
{


MapSFML::MapSFML(unsigned width, unsigned height)
	: Dimensions(width, height), m_map(width, height)
{

}

MapSFML::~MapSFML()
{
	//m_lookUpTexture->WriteTextureToFile();
	//m_maskmap->SaveToFile("untitledmask.png");
	//m_heightmap->SaveToFile("untitledHeight.png");
	//m_terrainmap->SaveToFile("untitledTerrain.png");
}

void MapSFML::GenerateHeightMap(const NoiseMapData& data)
{
	m_map.GenerateHeightMap(data);
}

void MapSFML::GenerateHeightMapTectonic()
{
	m_map.GenerateHeightMapTectonic();
}

void MapSFML::GenerateHeightMapTectonic(const NoiseMapData& data)
{
}

void MapSFML::GenerateTerrainMap(const std::vector<double>& noiseMap)
{
	m_map.GenerateTerrainMap(noiseMap);
}

void MapSFML::GenerateTerrainMap(const std::vector<double>& noiseMap, const std::vector<TerrainType>& types)
{
}

void MapSFML::RegenerateLookUp(const LookupMapData& data)
{
	m_map.RegenerateLookUp(data);
	rend::drawTileMap(m_map.GetLookupTileMap(), m_lookupTexture, Width(), Height());
}

void MapSFML::RegenerateLookupBorders(const LookupMapData& data)
{
	m_map.RegenerateLookupBorders(data);
	rend::drawTileMap(m_map.GetLookupTileMap(), m_lookupTexture, Width(), Height()); 
}

void MapSFML::GenerateMap(const std::vector<uint8_t>& textureBuffer, unsigned width, unsigned height)
{
	setDimensions(width, height);
	m_map.GenerateMap(textureBuffer, width, height);
	rend::drawTileMap(m_map.GetLookupTileMap(), m_lookupTexture, Width(), Height()); 
}

void MapSFML::GenerateMapFromHeigthMap(const std::vector<uint8_t>& textureBuffer, float cutOffHeight)
{
	m_map.GenerateMapFromHeigthMap(textureBuffer, cutOffHeight);
	rend::drawTileMap(m_map.GetLookupTileMap(), m_lookupTexture, Width(), Height()); 
}
void MapSFML::SaveMap(const std::string& filePath) const
{
	std::string name = filePath + "\\" + "Lookup Texture.png";
	rend::saveToFile(m_lookupTexture, name.c_str());
	//rend::saveToFile(m_lookupTextureLand, "Lookup Texture Land.png");
	//rend::saveToFile(m_lookupTextureOcean, "Lookup Texture Ocean.png");
	//rend::saveToFile(m_maskTexture, "Mask Texture.png");
	//rend::saveToFile(m_heighMapTexture, "HeigthMap Texture.png");
	//rend::saveToFile(m_terrainTexture, "Terain Texture.png");
}
}