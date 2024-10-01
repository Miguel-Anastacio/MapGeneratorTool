#include "Map.h"
#include "geometry/ComputeGeometry.h"
#include "../../thirdparty/lodepng/textureHandler.h"
#include "Renderer.h"
#include "../../thirdparty/fastNoiseLite/FastNoiseLite.h"
#include "components/HeightMap.h"
namespace MapGeneratorTool
{


Map::Map(unsigned width, unsigned height, int seeds, const char* lookUpTextureName, int iterLloyd)
	: Dimensions(width, height), m_lookupTextureName(lookUpTextureName)
{

}

Map::~Map()
{
	//m_lookUpTexture->WriteTextureToFile();
	//m_maskmap->SaveToFile("untitledmask.png");
	//m_heightmap->SaveToFile("untitledHeight.png");
	//m_terrainmap->SaveToFile("untitledTerrain.png");
}

void Map::GenerateHeightMap(const NoiseMapData& data)
{
	m_heightmap = std::make_unique<HeightMap>("heightMap1.png", data);
	m_terrainmap = std::make_unique<TerrainMap>("terrainMap.png", m_heightmap->NoiseMap(), data.width, data.height, m_terrainTypes);
	//rend::drawDiagram(m_terrainmap->Texture(), m_diagram, data.width, data.height);
}

void Map::GenerateTerrainMap(const std::vector<double>& noiseMap)
{
	m_terrainmap = std::make_unique<TerrainMap>("terrainMap.png", m_heightmap->NoiseMap(), m_heightmap->Width(), m_heightmap->Height(), m_terrainTypes);
	//rend::drawDiagram(m_terrainmap->Texture(), m_diagram, Width(), Height());
}

void Map::GenerateTerrainMap(const std::vector<double>& noiseMap, const std::vector<TerrainType>& types)
{
	m_terrainTypes = types;
	m_terrainmap = std::make_unique<TerrainMap>("terrainMap.png", m_heightmap->NoiseMap(), m_heightmap->Width(), m_heightmap->Height(), types);
	//rend::drawDiagram(m_terrainmap->Texture(), m_diagram, Width(), Height());
}

void Map::RegenerateLookUp(const LookupMapData& data)
{
	assert(m_lookupmap != nullptr);
	assert(m_landMask != nullptr);
	assert(m_oceanMask != nullptr);
	assert(m_maskmap != nullptr);

	if (std::abs(data.cutOffHeight - m_cutOffHeight) > 0.00001)
	{
		m_maskmap->RegenerateMask(data.cutOffHeight, true);
		m_landMask->RegenerateMask(data.cutOffHeight, true);
		m_oceanMask->RegenerateMask(data.cutOffHeight, false);
	}

	m_lookupmap->RegenerateLookUp(data, m_landMask.get(), m_oceanMask.get());

	m_maskmap->Texture().clear();
	rend::drawBuffer(m_maskmap->GetMaskBuffer(), m_maskmap->Texture(), Width(), Height());
}

void Map::GenerateMaskFromHeightMapTexture(const std::vector<uint8_t>& textureBuffer, float cutOffHeight)
{
	m_maskmap = std::make_unique<MapMask>("LandmassMaskTest.png", textureBuffer, Width(), Height(), cutOffHeight);
	m_landMask = std::make_unique<MapMask>("landMask.png", textureBuffer, Width(), Height(), cutOffHeight);
	m_oceanMask = std::make_unique<MapMask>("oceanMask.png", textureBuffer, Width(), Height(), cutOffHeight, false);

	m_heightmap = std::make_unique<HeightMap>("heightMap1.png", Width(), Height(), m_maskmap->MoveElevation());
	m_terrainmap = std::make_unique<TerrainMap>("terrainMap.png", m_heightmap->NoiseMap(), Width(), Height(), m_terrainTypes);
}

void Map::GenerateMap(const std::vector<uint8_t>& textureBuffer, float cutOffHeight)
{
	m_maskmap = std::make_unique<MapMask>("LandmassMaskTest.png", textureBuffer, Width(), Height(), cutOffHeight);
	m_cutOffHeight = cutOffHeight;
	m_landMask = std::make_unique<MapMask>("landMask.png", textureBuffer, Width(), Height(), cutOffHeight);
	m_oceanMask = std::make_unique<MapMask>("oceamMask.png", textureBuffer, Width(), Height(), cutOffHeight, false);

	m_lookupmap = std::make_unique<LookupMap>("lookupTexture.png", Width(), Height());
	RegenerateLookUp(LookupMapData(Width(), Height(), LookupFeatures(), LookupFeatures(), 0.001f));

	//m_lookupmap->SaveToFile();

	m_heightmap = std::make_unique<HeightMap>("heightMap1.png", Width(), Height(), m_landMask->GetElevation());
	m_terrainmap = std::make_unique<TerrainMap>("terrainMap.png", m_heightmap->NoiseMap(), Width(), Height(), m_terrainTypes);
}
void Map::SaveMap()
{
	SaveMapComponent(m_lookupmap.get());
	SaveMapComponent(m_heightmap.get());
	SaveMapComponent(m_terrainmap.get());
	SaveMapComponent(m_landMask.get());
	SaveMapComponent(m_maskmap.get());
	SaveMapComponent(m_oceanMask.get());
}
void Map::SaveMapComponent(MapComponent* component)
{
	if (component)
		component->SaveToFile();
	else
		std::cout << "ERROR saving map component\n";
}
}