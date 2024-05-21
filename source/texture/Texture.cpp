#include "Texture.h"
#include "../../thirdparty/lodepng/textureHandler.h"
namespace MapGeneratorTool
{
	Texture::Texture(const char* filename)
	{
		ReadTextureFromFile(filename);
	}

	//Texture& Texture::operator =(const Texture& other)
	//{
	//	if (this == &other) {
	//		return *this; // Handle self-assignment
	//	}

	//	m_width = other.m_width;
	//	m_height = other.m_height;
	//	m_name = other.m_name;
	//	m_buffer = other.m_buffer;
	//}
	void Texture::ReadTextureFromFile(const char* filename)
	{
		unsigned width = 0, height = 0;
		m_buffer = textureHandler::decodeOneStep(filename, width, height);
		setDimensions(width, height);
	}
	void Texture::WriteTextureToFile(const char* filename) const
	{
		textureHandler::encodeOneStep(filename, m_buffer, width(), height());
	}
	void Texture::WriteTextureToFile() const
	{
		textureHandler::encodeOneStep(m_name, m_buffer, width(), height());
	}
	//Texture::Texture(int width, int height, const char* name);
}