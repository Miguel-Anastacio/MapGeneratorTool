#include "Map.h"
#include "../texture/Texture.h"
#include "../ComputeGeometry.h"
#include "../../thirdparty/lodepng/textureHandler.h"
#include "../utils/Renderer.h"
#include "../../thirdparty/fastNoiseLite/FastNoiseLite.h"
#include "HeightMap.h"

namespace MapGeneratorTool
{

//Map::Map(const char* maskFileName, int seeds, const char* lookUpTextureName, int iterLloyd)
//	:
//	m_divisions(valSeeds(seeds)), 
//	m_lookupTextureName(lookUpTextureName),
//	m_diagram(std::move(geomt::generateDiagram(geomt::generatePoints<double>(seeds))))
//{
//	// read mask
//	const Texture mask = Texture(maskFileName);
//	//m_lookUpTexture = std::make_unique<Texture>(mask.width(), mask.height(), lookUpTextureName);
//
//	setDimensions(mask.width(), mask.height());
//	CreateLookUpTextureFromMask(mask);
//}

Map::Map(unsigned width, unsigned height, int seeds, const char* lookUpTextureName, int iterLloyd)
	: Dimensions(width, height), m_lookupTextureName(lookUpTextureName),
	m_divisions(valSeeds(seeds)),
	m_diagram(std::move(geomt::generateDiagram(geomt::generatePoints<double>(seeds))))
{
	geomt::lloydRelaxation(m_diagram, iterLloyd);
	m_lookupTexture.create(width, height);
	//rend::drawPolygons(m_diagram.GetPolygons(), m_lookupTexture, width, height);
	rend::drawDiagram(m_lookupTexture, m_diagram, width, height);

}

Map::~Map()
{
	//m_lookUpTexture->WriteTextureToFile();
	SaveLookupMapToFile();
	m_heightmap->SaveToFile("untitledHeight.png");
	m_terrainmap->SaveToFile("untitledTerrain.png");
}

void Map::GenerateMap(const LookupMapData& data)
{
	m_diagram = std::move(geomt::generateDiagram(geomt::generatePoints<double>(data.numberOfSeeds, data.seed)));
	geomt::lloydRelaxation(m_diagram, data.lloyd);
	m_lookupTexture.clear();
	m_lookupTexture.create(data.width, data.height);
	//rend::drawPolygons(m_diagram.GetPolygons(), m_lookupTexture, data.width, data.height);
	rend::drawDiagram(m_lookupTexture, m_diagram, data.width, data.height);

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

void Map::CreateLookUpTextureFromMask(const Texture& mask)
{
	//const std::vector<uint8_t> maskData = GenerateMaskData(mask);
	//	
	//const std::vector<Point> seeds = SimpleVoronoiDiagram::GenerateSeeds(m_divisions, width(), height());
	//const std::vector<Point> diagram = SimpleVoronoiDiagram::GenerateDiagramFromMask(seeds, width(), height(), maskData);

	//const std::unordered_map<Point, Color> colorMap = SimpleVoronoiDiagram::GenerateColorMap(seeds);

	////PopulateTexture(colorMap, diagram, m_lookUpTexture.get());
	//OutputSeedPoints(seeds);
}

std::vector<uint8_t> Map::GenerateMaskData(const Texture& mask) const
{
	std::vector<uint8_t> maskData(width() * height());
	const std::vector<uint8_t> bufferMask = mask.GetBuffer();
	for (unsigned y = 0; y < height(); y++)
	{
		for (unsigned x = 0; x < width(); x++)
		{
			int index = 4 * width() * y + 4 * x;
			Color maskPixelColor(bufferMask[index], bufferMask[index + 1], bufferMask[index + 2], 255);

			// if white then this pixel is water
			if (maskPixelColor == Color(255, 255, 255, 255))
			{
				maskData[y * width() + x] = 0;
			}
			else 
			{
				// black is land
				maskData[y * width() + x] = 1;
			}

		}
	}
	return maskData;
}

void Map::OutputSeedPoints(const std::vector<Point>& seeds) const
{
	std::vector<uint8_t> image(width() * height() * 4);
	for (int y = 0; y < height(); y++)
	{
		for (int x = 0; x < width(); x++)
		{
			image[4 * width() * y + 4 * x + 0] = 0;
			image[4 * width() * y + 4 * x + 1] = 0;
			image[4 * width() * y + 4 * x + 2] = 0;
			image[4 * width() * y + 4 * x + 3] = 255;
		}
	}

	for (auto& pt : seeds)
	{
		int index = 4 * width() * pt.Y + 4 * pt.X;
		image[index] = 255;
		image[index+1] = 0;
		image[index+2] = 0;
		image[index+3] = 255;
	}

	textureHandler::encodeOneStep("seeds.png", image, width(), height());

}

void Map::SaveLookupMapToFile() const
{
	/*MapGeneratorTool::drawPolygons(m_diagram.GetPolygons(), m_lookupTexture, width(), height());*/
	rend::saveToFile(m_lookupTexture, m_lookupTextureName);
}

void Map::SaveLookupMapToFile(const char* filename) const
{
	rend::saveToFile(m_lookupTexture, filename);
}

void Map::SaveHeightMapToFile(const char* filename) const
{
	m_heightmap->SaveToFile(filename);
}

void Map::PopulateTexture(const std::unordered_map<Point, Color>& colorMap, const std::vector<Point>& diagram, Texture* texture) const
{
	std::vector<uint8_t> image(width() * height() * 4);
	for (int y = 0; y < height(); y++)
	{
		for (int x = 0; x < width(); x++)
		{
			Point pt = diagram[width() * y + x];
			image[4 * width() * y + 4 * x + 0] = colorMap.at(pt).R;
			image[4 * width() * y + 4 * x + 1] = colorMap.at(pt).G;
			image[4 * width() * y + 4 * x + 2] = colorMap.at(pt).B;
			image[4 * width() * y + 4 * x + 3] = 255;
		}
	}
	texture->SetBuffer(image);

}
}