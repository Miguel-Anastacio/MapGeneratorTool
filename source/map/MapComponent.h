#pragma once
#include "Utils.h"
#include <SFML/Graphics/RenderTexture.hpp>
namespace MapGeneratorTool
{
class MapComponent : public Utils::Dimensions
{
public:
	MapComponent(unsigned width, unsigned height, const char* name) : Dimensions(width, height), m_name(name) {};
	virtual ~MapComponent() = default;

	void SaveToFile() const;
	void SaveToFile(const char* filename) const;

	inline const sf::RenderTexture& Texture() const
	{
		return m_texture;
	}

	inline sf::RenderTexture& Texture()
	{
		return m_texture;
	}


protected:
	sf::RenderTexture m_texture;
	const char* m_name;
};


}