#pragma once
#include <vector>
#include "Color.h"
#include <queue>
#include "Utils.h"

namespace MapGeneratorTool
{
class Mask : public Utils::Dimensions
{
public:

	Mask(unsigned wid, unsigned ht, const std::vector<uint8_t>& textureBuffer, const Utils::Color& maskColor = Utils::Color(255, 255, 255, 255))
		: Dimensions(wid, ht), data(wid* ht)
	{
		SetMaskData(textureBuffer, maskColor);
	}

	Mask(unsigned wid, unsigned ht, const Utils::Color& maskColor = Utils::Color(255, 255, 255, 255))
		: Dimensions(wid, ht), data(wid* ht)
	{
		
	}

    bool isInMask(int x, int y) const 
	{
        auto width = Width();
        auto height = Height();
		if (x < 0 || x >= width || y < 0 || y >= height)
			return false;

        return data[y * width + x];
    }

	void SetMaskData(const std::vector<uint8_t>& buffer, const Utils::Color& maskColor)
	{	
        auto width = Width();
        auto height = Height();
		for (unsigned y = 0; y < height; y++)
		{
			for (unsigned x = 0; x < width; x++)
			{
				int index = 4 * width * y + 4 * x;
				Utils::Color maskPixelColor(buffer[index], buffer[index + 1], buffer[index + 2], 255);
				if (maskPixelColor == maskColor)
				{
					data[y * width + x] = true;
				}
				else
				{
					data[y * width + x] = false;
				}

			}
		}
	}

private:
    std::vector<bool> data; 
};

}