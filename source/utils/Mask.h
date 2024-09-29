#pragma once
#include <vector>
#include "Color.h"
namespace MapGeneratorTool
{
struct Mask 
{
    unsigned width = 1024, height = 1024;
    std::vector<bool> data; // True for land, False for water

	Mask(unsigned wid, unsigned ht, const std::vector<uint8_t>& textureBuffer)
		: width(wid), height(ht), data(width*height)
	{
		SetMaskData(textureBuffer);
	}

    bool isLand(int x, int y) const 
	{
        return data[y * width + x];
    }

	void SetMaskData(const std::vector<uint8_t>& buffer) 
	{
		
		for (unsigned y = 0; y < height; y++)
		{
			for (unsigned x = 0; x < width; x++)
			{
				int index = 4 * width * y + 4 * x;
				Utils::Color maskPixelColor(buffer[index], buffer[index + 1], buffer[index + 2], 255);
				// if white then this pixel is water
				if (maskPixelColor == Utils::Color(255, 255, 255, 255))
				{
					data[y * width + x] = false;
				}
				else
				{
					// black is land
					data[y * width + x] = true;
				}

			}
		}
	}
};

}