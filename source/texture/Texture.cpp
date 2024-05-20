#include "Texture.h"
#include "../../thirdparty/lodepng/textureHandler.h"
namespace MapGeneratorTool
{
	Texture::Texture(const char* filename)
	{
		ReadTextureFromFile(filename);
	}
	void Texture::ReadTextureFromFile(const char* filename)
	{
		//m_buffer = textureHandler::decodeOneStep(filename, m_width, m_height);
	}
	void Texture::WriteTextureToFile(const char* filename) const
	{
		textureHandler::encodeOneStep(filename, m_buffer, m_width, m_height);
	}
	void Texture::WriteTextureToFile() const
	{
		textureHandler::encodeOneStep(m_name, m_buffer, m_width, m_height);
	}
	//Texture::Texture(int width, int height, const char* name);
}