#include "HeightMap.h"
namespace MapGeneratorTool
{
	HeightMap::HeightMap(unsigned width, unsigned height, const char* name, double noiseScale, const siv::PerlinNoise& noise) 
		: m_texture(width, height, name), m_noiseScale(noiseScale), m_noise(noise)
	{
		CreateHeightMap(width, height, noiseScale, noise);
	}
		
	HeightMap::~HeightMap()
	{

	}

	void HeightMap::SaveHeightMapToFile()
	{
		unsigned width = m_texture.width();
		unsigned height = m_texture.height();
		std::vector<uint8_t> image(width * height * 4);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				double value = m_noiseValues[width * y + x];
				image[4 * width * y + 4 * x + 0] = value * 255;
				image[4 * width * y + 4 * x + 1] = value * 255;
				image[4 * width * y + 4 * x + 2] = value * 255;
				image[4 * width * y + 4 * x + 3] = 255;
			}
		}

		m_texture.SetBuffer(image);
		m_texture.WriteTextureToFile();
	}

	void HeightMap::CreateHeightMap(unsigned width, unsigned height, double noiseScale, const siv::PerlinNoise& noise)
    {
		if (noiseScale <= 0)
		{
			noiseScale = 0.0001f;
		}
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				double xSample = (double)x / noiseScale;
				double ySample = (float)y / noiseScale;

				m_noiseValues[y * width + x] = noise.noise2D_01(xSample, ySample);
			}
		}
		
    }



}