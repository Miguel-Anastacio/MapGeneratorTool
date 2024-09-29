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

		inline std::vector<double> MoveElevation() 
		{
			return std::move(m_elevation);
		}

		inline std::vector<uint8_t> GetMaskBuffer()
		{
			return m_buffer;
		}
	
	private:
		//std::vector<double> CreateHeightMap(const NoiseMapData& data) const;
		std::vector<double> ExtractHeightMapFromTexture(const std::vector<uint8_t>& buffer, unsigned width, unsigned height) const;
		std::vector<uint8_t> CreateBuffer(const std::vector<double>& data, float cutOffHeight) const;

		
		std::vector<double> m_elevation;
		std::vector<uint8_t> m_buffer;
	};


}