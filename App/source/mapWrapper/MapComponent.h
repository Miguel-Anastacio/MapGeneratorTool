#pragma once
#include "data/Utils.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include "map/MapComponent.h"
namespace MapGeneratorTool
{
class MapComponentSFML
{
public:
	MapComponentSFML(unsigned width, unsigned height, const char* name)  : m_mapData(width, height, name)
	{
		//m_texture.create(width, height)
		m_mapData = std::make_
	};
	virtual ~MapComponentSFML() = default;

	void SaveToFile() const;
	void SaveToFile(const char* filename) const;
	void SaveToFile(const std::string& filepath) const;

	inline const sf::RenderTexture& Texture() const
	{
		return m_texture;
	}

	inline sf::RenderTexture& Texture()
	{
		return m_texture;
	}

	const char* Name() const
	{
		return m_mapData->Name();
	}

	void Clear()
	{

	}

protected:
	sf::RenderTexture m_texture;
	std::unique_ptr<MapGenerator::MapComponent> m_mapData;

};


}