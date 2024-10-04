#pragma once
#include "Utils.h"
#include <SFML/Graphics/RenderTexture.hpp>
namespace MapGeneratorTool
{
class MapComponent : public Utils::Dimensions
{
public:
	MapComponent(unsigned width, unsigned height, const char* name) : Dimensions(width, height), m_name(name) 
	{
		//m_texture.create(width, height)
	};
	virtual ~MapComponent() = default;

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



	void Clear()
	{

	}


protected:
	sf::RenderTexture m_texture;
	const char* m_name;
};


}