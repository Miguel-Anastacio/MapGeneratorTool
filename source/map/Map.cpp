#include "Map.h"
#include "geometry/ComputeGeometry.h"
#include "../../thirdparty/lodepng/textureHandler.h"
#include "Renderer.h"
#include "../../thirdparty/fastNoiseLite/FastNoiseLite.h"
#include "components/HeightMap.h"
#include "Mask.h"
namespace MapGeneratorTool
{


Map::Map(unsigned width, unsigned height, int seeds, const char* lookUpTextureName, int iterLloyd)
	: Dimensions(width, height), m_lookupTextureName(lookUpTextureName),
	m_divisions(valSeeds(seeds)),
	m_diagram(std::move(geomt::generateDiagram(geomt::generatePoints<double>(seeds))))
{
	//geomt::lloydRelaxation(m_diagram, iterLloyd);
	m_lookupTexture.create(width, height);
	//rend::drawPolygons(m_diagram.GetPolygons(), m_lookupTexture, width, height);
	//rend::drawDiagram(m_lookupTexture, m_diagram, width, height);

}

Map::~Map()
{
	//m_lookUpTexture->WriteTextureToFile();
	//m_maskmap->SaveToFile("untitledmask.png");
	m_heightmap->SaveToFile("untitledHeight.png");
	//m_terrainmap->SaveToFile("untitledTerrain.png");
	rend::saveToFile(m_lookupTexture, "untitledLookup.png");
}

void Map::GenerateLookupMap(const LookupMapData& data)
{
	GenerateLookupMapFromMask(data, m_maskmap->GetMaskBuffer());
	GenerateTerrainMap(m_heightmap->NoiseMap());
}

void Map::GenerateHeightMap(const NoiseMapData& data)
{
	m_heightmap = std::make_unique<HeightMap>("heightMap1.png", data);
	m_terrainmap = std::make_unique<TerrainMap>("terrainMap.png", m_heightmap->NoiseMap(), data.width, data.height, m_terrainTypes);
	//rend::drawDiagram(m_terrainmap->Texture(), m_diagram, data.width, data.height);
}

void Map::GenerateTerrainMap(const std::vector<double>& noiseMap)
{
	m_terrainmap = std::make_unique<TerrainMap>("terrainMap.png", m_heightmap->NoiseMap(), m_heightmap->width(), m_heightmap->height(), m_terrainTypes);
	//rend::drawDiagram(m_terrainmap->Texture(), m_diagram, width(), height());
}

void Map::GenerateTerrainMap(const std::vector<double>& noiseMap, const std::vector<TerrainType>& types)
{
	m_terrainTypes = types;
	m_terrainmap = std::make_unique<TerrainMap>("terrainMap.png", m_heightmap->NoiseMap(), m_heightmap->width(), m_heightmap->height(), types);
	//rend::drawDiagram(m_terrainmap->Texture(), m_diagram, width(), height());
}

void Map::RegenerateLookUp(const LookupMapData& data)
{
	std::vector<uint8_t> llmBuffer = GenerateLookupMapFromMask(LookupMapData(width(), height(), data.seed, data.numberOfSeeds, data.lloyd), m_landMask->GetMaskBuffer(), "landMaskLookUp.png");
	std::vector<uint8_t> lloBuffer = GenerateLookupMapFromMask(LookupMapData(width(), height(), data.seed, data.numberOfSeeds, data.lloyd), m_oceanMask->GetMaskBuffer(), "oceanMaskLookUp.png");

	std::vector<uint8_t> temp(width() * height() * 4);
	for (int y = 0; y < height(); y++)
	{
		for (int x = 0; x < width(); x++)
		{
			unsigned int index = (y * width() + x) * 4;

			if (llmBuffer[index + 3] > lloBuffer[index + 3])
			{
				temp[index + 0] = llmBuffer[index];
				temp[index + 1] = llmBuffer[index + 1];
				temp[index + 2] = llmBuffer[index + 2];
				temp[index + 3] = llmBuffer[index + 3];
			}
			else
			{
				temp[index + 0] = lloBuffer[index];
				temp[index + 1] = lloBuffer[index + 1];
				temp[index + 2] = lloBuffer[index + 2];
				temp[index + 3] = lloBuffer[index + 3];
			}
		}
	}

	m_lookupTexture.clear();
	rend::drawBuffer(temp, m_lookupTexture, width(), height());
	//rend::drawPoints(m_lookupTexture, diagram, data.width, data.height);
	rend::saveToFile(m_lookupTexture, "finalLookup.png");
}

std::vector<uint8_t> Map::GenerateLookupMapFromMask(const LookupMapData& data, const std::vector<uint8_t>& buffer, const char* name)
{
	Mask mask(data.width, data.height, buffer);
	auto pointsCOntr = geomt::generatePointsConstrained<double>(data.numberOfSeeds, data.seed, true, mask);
	mygal::Diagram<double>  diagram = std::move(geomt::generateDiagram(geomt::generatePointsConstrained<double>(data.numberOfSeeds, data.seed, false, mask)));
	mygal::Diagram<double>  diagramNorm = std::move(geomt::generateDiagram(geomt::generatePoints<double>(data.numberOfSeeds, data.seed)));
	geomt::lloydRelaxation(diagram, data.lloyd);


	//m_lookupTexture.clear();
	//m_lookupTexture.create(data.width, data.height);
	////rend::drawBuffer(buffer, m_lookupTexture, data.width, data.height);
	////rend::drawDiagram(m_lookupTexture, diagram, data.width, data.height);
	//rend::drawPolygons(diagram.GetPolygons(), m_lookupTexture, data.width, data.height);
	////rend::drawPoints(m_lookupTexture, diagram, data.width, data.height);
	//rend::saveToFile(m_lookupTexture, "testPolygons.png");
	sf::RenderTexture texture;
	texture.clear();
	texture.create(data.width, data.height);
	//rend::drawBuffer(buffer, m_lookupTexture, data.width, data.height);
	//rend::drawDiagram(m_lookupTexture, diagram, data.width, data.height);
	rend::drawPolygons(diagram.GetPolygons(), texture, data.width, data.height);
	sf::Image image = texture.getTexture().copyToImage();

	// Get the pixel array (RGBA values)
	const sf::Uint8* pixels = image.getPixelsPtr();


	std::vector<uint8_t> temp(width() * height() * 4);
	for (int y = 0; y < height(); y++)
	{
		for (int x = 0; x < width(); x++)
		{
			unsigned int index = (y * width() + x) * 4;
			temp[index + 0] = pixels[index] * (buffer[index + 0] / 255);
			temp[index + 1] = pixels[index + 1] * (buffer[index + 1] / 255);
			temp[index + 2] = pixels[index + 2] * (buffer[index + 2] / 255);
			temp[index + 3] = pixels[index + 3] * (buffer[index + 3] / 255);
		}
	}
	texture.clear();
	rend::drawBuffer(temp, texture, width(), height());
	//rend::drawPoints(m_lookupTexture, diagram, data.width, data.height);
	rend::saveToFile(texture, name);

	return temp;
	

}

void Map::GenerateMaskFromHeightMapTexture(const std::vector<uint8_t>& textureBuffer, float cutOffHeight)
{
	m_maskmap = std::make_unique<MapMask>("LandmassMaskTest.png", textureBuffer, width(), height(), cutOffHeight);
	m_landMask = std::make_unique<MapMask>("landMask.png", textureBuffer, width(), height(), cutOffHeight);
	m_oceanMask = std::make_unique<MapMask>("oceanMask.png", textureBuffer, width(), height(), cutOffHeight, false);

	m_heightmap = std::make_unique<HeightMap>("heightMap1.png", width(), height(), m_maskmap->MoveElevation());
	m_terrainmap = std::make_unique<TerrainMap>("terrainMap.png", m_heightmap->NoiseMap(), width(), height(), m_terrainTypes);
}

void Map::GenerateMap(const std::vector<uint8_t>& textureBuffer, float cutOffHeight)
{
	//m_maskmap = std::make_unique<MapMask>("LandmassMaskTest.png", textureBuffer, width(), height(), cutOffHeight);
	m_landMask = std::make_unique<MapMask>("landMask.png", textureBuffer, width(), height(), cutOffHeight);
	m_oceanMask = std::make_unique<MapMask>("oceamMask.png", textureBuffer, width(), height(), cutOffHeight, false);

	std::vector<uint8_t> llmBuffer = GenerateLookupMapFromMask(LookupMapData(width(), height(), 0, 20, 10), m_landMask->GetMaskBuffer(), "landMaskLookUp.png");
	std::vector<uint8_t> lloBuffer = GenerateLookupMapFromMask(LookupMapData(width(), height(), 0, 20, 10), m_oceanMask->GetMaskBuffer(), "oceanMaskLookUp.png");

	std::vector<uint8_t> temp(width() * height() * 4);
	for (int y = 0; y < height(); y++)
	{
		for (int x = 0; x < width(); x++)
		{
			unsigned int index = (y * width() + x) * 4;

			if (llmBuffer[index + 3] > lloBuffer[index + 3])
			{
				temp[index + 0] = llmBuffer[index];
				temp[index + 1] = llmBuffer[index + 1];
				temp[index + 2] = llmBuffer[index + 2]; 
				temp[index + 3] = llmBuffer[index + 3];
			}
			else 
			{
				temp[index + 0] = lloBuffer[index];
				temp[index + 1] = lloBuffer[index + 1];
				temp[index + 2] = lloBuffer[index + 2];
				temp[index + 3] = lloBuffer[index + 3];
			}
		}
	}

	m_lookupTexture.clear();
	rend::drawBuffer(temp, m_lookupTexture, width(), height());
	//rend::drawPoints(m_lookupTexture, diagram, data.width, data.height);
	rend::saveToFile(m_lookupTexture, "finalLookup.png");


	m_heightmap = std::make_unique<HeightMap>("heightMap1.png", width(), height(), m_landMask->MoveElevation());
	m_terrainmap = std::make_unique<TerrainMap>("terrainMap.png", m_heightmap->NoiseMap(), width(), height(), m_terrainTypes);
}

void Map::CreateLookUpTextureFromMask(const Texture& mask)
{
	//	
	//const std::vector<Point> seeds = SimpleVoronoiDiagram::GenerateSeeds(m_divisions, width(), height());
	//const std::vector<Point> diagram = SimpleVoronoiDiagram::GenerateDiagramFromMask(seeds, width(), height(), maskData);

	//const std::unordered_map<Point, Color> colorMap = SimpleVoronoiDiagram::GenerateColorMap(seeds);

	////PopulateTexture(colorMap, diagram, m_lookUpTexture.get());
	//OutputSeedPoints(seeds);
}


//void Map::OutputSeedPoints(const std::vector<Point>& seeds) const
//{
//	std::vector<uint8_t> image(width() * height() * 4);
//	for (int y = 0; y < height(); y++)
//	{
//		for (int x = 0; x < width(); x++)
//		{
//			image[4 * width() * y + 4 * x + 0] = 0;
//			image[4 * width() * y + 4 * x + 1] = 0;
//			image[4 * width() * y + 4 * x + 2] = 0;
//			image[4 * width() * y + 4 * x + 3] = 255;
//		}
//	}
//
//	for (auto& pt : seeds)
//	{
//		int index = 4 * width() * pt.Y + 4 * pt.X;
//		image[index] = 255;
//		image[index+1] = 0;
//		image[index+2] = 0;
//		image[index+3] = 255;
//	}
//
//	textureHandler::encodeOneStep("seeds.png", image, width(), height());
//
//}
//
//void Map::SaveLookupMapToFile() const
//{
//	/*MapGeneratorTool::drawPolygons(m_diagram.GetPolygons(), m_lookupTexture, width(), height());*/
//	rend::saveToFile(m_lookupTexture, m_lookupTextureName);
//}
//
//void Map::SaveLookupMapToFile(const char* filename) const
//{
//	rend::saveToFile(m_lookupTexture, filename);
//}
//
//void Map::SaveHeightMapToFile(const char* filename) const
//{
//	m_heightmap->SaveToFile(filename);
//}

//void Map::PopulateTexture(const std::unordered_map<Point, Color>& colorMap, const std::vector<Point>& diagram, Texture* texture) const
//{
//	std::vector<uint8_t> image(width() * height() * 4);
//	for (int y = 0; y < height(); y++)
//	{
//		for (int x = 0; x < width(); x++)
//		{
//			Point pt = diagram[width() * y + x];
//			image[4 * width() * y + 4 * x + 0] = colorMap.at(pt).R;
//			image[4 * width() * y + 4 * x + 1] = colorMap.at(pt).G;
//			image[4 * width() * y + 4 * x + 2] = colorMap.at(pt).B;
//			image[4 * width() * y + 4 * x + 3] = 255;
//		}
//	}
//	texture->SetBuffer(image);
//
//}
}