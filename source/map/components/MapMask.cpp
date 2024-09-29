#pragma once
#include <algorithm>
#include "../../thirdparty/MyGAL/Vector2.h"
#include "Renderer.h"
#include "components/MapMask.h"
#include "MapMask.h"
namespace MapGeneratorTool
{
	MapMask::MapMask(const char* name, const std::vector<uint8_t>& textureBuffer, unsigned width, unsigned height, float cutOffHeight)
		: MapComponent(width, height, name)
	{
		//elevation = ExtractHeightMapFromTexture(textureBuffer, width, height);
		rend::drawBuffer(CreateBuffer(ExtractHeightMapFromTexture(textureBuffer, width, height), cutOffHeight), m_texture, width, height);
	}

	std::vector<uint8_t> MapMask::CreateBuffer(const std::vector<float>& data, float cutOffHeight) const
	{
		unsigned width = this->width();
		unsigned height = this->height();
		std::vector<uint8_t> image(width * height * 4);
		for (unsigned y = 0; y < height; y++)
		{
			for (unsigned x = 0; x < width; x++)
			{
				double value = data[width * y + x];

				if (value < cutOffHeight)
				{
					image[4 * width * y + 4 * x + 0] = 255;
					image[4 * width * y + 4 * x + 1] = 255;
					image[4 * width * y + 4 * x + 2] = 255;
					image[4 * width * y + 4 * x + 3] = 255;
				}
				else
				{
					image[4 * width * y + 4 * x + 0] = 0;
					image[4 * width * y + 4 * x + 1] = 0;
					image[4 * width * y + 4 * x + 2] = 0;
					image[4 * width * y + 4 * x + 3] = 255;
				}
			}
		}
		return image;
	}

	std::vector<float> MapGeneratorTool::MapMask::ExtractHeightMapFromTexture(const std::vector<uint8_t>& buffer, unsigned width, unsigned height) const
	{
		//assert(buffer.size() == (width * height * 4);

		std::vector<float> elevation(width * height);
		for (unsigned y = 0; y < height; y++)
		{
			for (unsigned x = 0; x < width; x++)
			{
				int index = 4 * width * y + 4 * x;

				elevation[width * y + x] = static_cast<float>(buffer[index]) / 255.f;
			}
		}

		return elevation;
	}

}