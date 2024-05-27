#pragma once
#include <algorithm>
#include "../../thirdparty/MyGAL/Vector2.h"
#include "../utils/Renderer.h"
#include "TerrainMap.h"
namespace MapGeneratorTool
{
	TerrainMap::TerrainMap(const char* name, const std::vector<double>& data, unsigned width, unsigned height)
		: MapComponent(width, height, name)
	{
		m_terrainTypes.push_back(TerrainType(0.5, sf::Color::Blue, "Water"));
		m_terrainTypes.push_back(TerrainType(0.85, sf::Color::Green, "Grass"));
		m_terrainTypes.push_back(TerrainType(1.0, sf::Color::Yellow, "Rock"));
		RegenerateTerrain(data);
	}

	void TerrainMap::RegenerateTerrain(const std::vector<double>& data)
	{
		rend::drawBuffer(CreateBuffer(data), m_texture, this->width(), this->height());
	}

	
	std::vector<sf::Uint8> TerrainMap::CreateBuffer(const std::vector<double>& data) const
	{
		unsigned width = this->width();
		unsigned height = this->height();
		std::vector<sf::Uint8> image(width * height * 4);
		for (unsigned y = 0; y < height; y++)
		{
			for (unsigned x = 0; x < width; x++)
			{
				double value = data[width * y + x];

				sf::Color color = GetColorFromHeight(m_terrainTypes, data[width * y + x]);

				image[4 * width * y + 4 * x + 0] = color.r;
				image[4 * width * y + 4 * x + 1] = color.g;
				image[4 * width * y + 4 * x + 2] = color.b;
				image[4 * width * y + 4 * x + 3] = 255;
			}
		}
		return image;
	}



	sf::Color TerrainMap::GetColorFromHeight(const std::vector<TerrainType>& terrainTypes, double height) const
	{
		
		for (auto ter : terrainTypes)
		{
			if (height <= ter.minHeight) 
				return ter.color;
		}
	}

}