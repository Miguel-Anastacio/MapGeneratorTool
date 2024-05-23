#include "Map.h"
#include "../texture/Texture.h"
#include "../SimpleVoronoiDiagram.h"
#include "../../thirdparty/lodepng/textureHandler.h"

namespace MapGeneratorTool
{
	Map::Map(const char* maskFileName, int seeds, const char* lookUpTextureName)
		:
		m_divisions(valSeeds(seeds))
	{
		// read mask
		const Texture mask = Texture(maskFileName);
		m_lookUpTexture = std::make_unique<Texture>(mask.width(), mask.height(), lookUpTextureName);

		setDimensions(mask.width(), mask.height());

		CreateLookUpTextureFromMask(mask);
	}
	Map::Map(unsigned width, unsigned height, int seeds, const char* lookUpTextureName)
		: Dimensions(width, height), m_lookUpTexture(std::make_unique<Texture>(width, height, lookUpTextureName)),
		m_divisions(valSeeds(seeds))
	{
		CreateLookUpTexture();
	}

	Map::~Map()
	{
		m_lookUpTexture->WriteTextureToFile();
	}

	void Map::CreateLookUpTexture()
	{
		std::vector<Point> seeds = SimpleVoronoiDiagram::GenerateSeeds(m_divisions, width(), height());
		int iterations = 2;
		for (int i = 0; i < iterations; i++)
		{
			const std::vector<Point> diagram = SimpleVoronoiDiagram::GenerateDiagram(seeds, width(), height());
			seeds = SimpleVoronoiDiagram::ComputeCentroids(diagram, width(), height(), seeds);
		}

		const std::vector<Point> finalDiagram = SimpleVoronoiDiagram::GenerateDiagram(seeds, width(), height());
		const std::unordered_map<Point, Color> colorMap = SimpleVoronoiDiagram::GenerateColorMap(seeds);
		
		PopulateTexture(colorMap, finalDiagram, m_lookUpTexture.get());
		OutputSeedPoints(seeds);



	}

	void Map::CreateLookUpTextureFromMask(const Texture& mask)
	{
		const std::vector<uint8_t> maskData = GenerateMaskData(mask);
		
		const std::vector<Point> seeds = SimpleVoronoiDiagram::GenerateSeeds(m_divisions, width(), height());
		const std::vector<Point> diagram = SimpleVoronoiDiagram::GenerateDiagramFromMask(seeds, width(), height(), maskData);

		const std::unordered_map<Point, Color> colorMap = SimpleVoronoiDiagram::GenerateColorMap(seeds);

		PopulateTexture(colorMap, diagram, m_lookUpTexture.get());
		OutputSeedPoints(seeds);
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