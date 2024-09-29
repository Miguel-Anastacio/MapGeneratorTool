#pragma once
#include <vector>
#include <cassert>
#include <string>
#include "MapComponent.h"
#include "MapSpecs.h"
namespace MapGeneratorTool
{
	using namespace Utils;
	class  MapMask : public MapComponent
	{
	public:
		MapMask(const char* name, const std::vector<uint8_t>& textureBuffer, unsigned width, unsigned height, float cutOffHeight);
		//HeightMap(const Texture& texture, double noiseScale, const siv::PerlinNoise& noise, const NoiseSpecs& specs);
	
	private:
		//std::vector<double> CreateHeightMap(const NoiseMapData& data) const;
		std::vector<float> ExtractHeightMapFromTexture(const std::vector<uint8_t>& buffer, unsigned width, unsigned height) const;
		std::vector<uint8_t> CreateBuffer(const std::vector<float>& data, float cutOffHeight) const;



	};


}