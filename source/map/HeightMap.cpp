#include "HeightMap.h"
#include "../../thirdparty/fastNoiseLite/FastNoiseLite.h"
#include "../utils/Point.h"
namespace MapGeneratorTool
{
	HeightMap::HeightMap(unsigned width, unsigned height, const char* name, double noiseScale, const FastNoiseLite& noise, const NoiseSpecs& specs)
		: m_texture(std::make_unique<Texture>(width, height, name))/*, 
		m_noiseScale(noiseScale), m_noise(noise), m_noiseSpecs(specs)*/
	{
		m_noiseValues = CreateHeightMap(width, height, noiseScale, noise, specs);
		SaveHeightMapToFile();
	}
		
	//HeightMap::HeightMap(const Texture& texture, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& specs)
	//	: m_texture(texture), m_noiseScale(noiseScale), m_noise(noise), m_noiseSpecs(specs)
	//{

	//}

	HeightMap::~HeightMap()
	{

	}

	void HeightMap::SaveHeightMapToFile()
	{
		unsigned width = m_texture->width();
		unsigned height = m_texture->height();
		std::vector<uint8_t> image(width * height * 4);
		for (unsigned y = 0; y < height; y++)
		{
			for (unsigned x = 0; x < width; x++)
			{
				double value = m_noiseValues[width * y + x];
				image[4 * width * y + 4 * x + 0] = value * 255;
				image[4 * width * y + 4 * x + 1] = value * 255;
				image[4 * width * y + 4 * x + 2] = value * 255;
				image[4 * width * y + 4 * x + 3] = 255;
			}
		}

		m_texture->SetBuffer(image);
		m_texture->WriteTextureToFile();
	}

	void HeightMap::RegenerateHeightMap(double noiseScale, const FastNoiseLite& noise, const NoiseSpecs& specs)
	{
		//m_noiseSpecs = specs;
		//m_noiseScale = noiseScale;
		m_noiseValues = CreateHeightMap(m_texture->width(), m_texture->height(), noiseScale, noise, specs);
		SaveHeightMapToFile();
	}

	std::vector<double> HeightMap::CreateHeightMap(unsigned width, unsigned height, double noiseScale, const FastNoiseLite& noise, const NoiseSpecs& specs)
    {
		std::vector<double> heightMap(width * height);
		if (noiseScale <= 0)
		{
			noiseScale = 0.0001f;
		}

		float minNoise = 40000.f;
		float maxNoise = -40000.f;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float amplitude = 1;
				float frequency = 1;
				float noiseHeight = 0;

				for (int i = 0; i < specs.octaves(); i++)
				{
					double xSample = (double)x / noiseScale * frequency;
					double ySample = (float)y / noiseScale * frequency;

					noiseHeight += noise.GetNoise(xSample, ySample);
					amplitude *= specs.persistance();
					frequency *= specs.lacunarity();
				}

				if (noiseHeight > maxNoise)
					maxNoise = noiseHeight;
				else if (noiseHeight < minNoise)
					minNoise = noiseHeight;

				heightMap[y * width + x] = noiseHeight;
			}
		}

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				heightMap[y * width + x] = inverseLerp(minNoise, maxNoise, heightMap[y * width + x]);
			}
		}

		return heightMap;
    }



}