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

	bool isInMask(int x, int y) const;

	void SetMaskData(const std::vector<uint8_t>& buffer, const Utils::Color& maskColor);

private:
    std::vector<bool> data; 
};

}