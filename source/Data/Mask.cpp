#include "Mask.h"
namespace MapGeneratorTool
{
	bool Mask::isInMask(int x, int y) const
	{
		auto width = Width();
		auto height = Height();
		if (x < 0 || x >= (int)width || y < 0 || y >= (int)height)
			return false;

		return data[y * width + x];
	}

	void Mask::SetMaskData(const std::vector<uint8_t>& buffer, const Utils::Color& maskColor)
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
} // namespace MapGeneratorTool
