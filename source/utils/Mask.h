#pragma once
#include <vector>
#include "Color.h"
#include <MyGAL/Vector2.h>
#include <queue>
#include "Utils.h"

namespace MapGeneratorTool
{
struct Mask 
{
    unsigned width = 1024, height = 1024;
    std::vector<bool> data; // True for land, False for water

	Mask(unsigned wid, unsigned ht, const std::vector<uint8_t>& textureBuffer, const Utils::Color& maskColor = Utils::Color(255, 255, 255, 255))
		: width(wid), height(ht), data(width*height)
	{
		SetMaskData(textureBuffer, maskColor);
	}

    bool isInMask(int x, int y) const 
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return false;

        return data[y * width + x];
    }

	void SetMaskData(const std::vector<uint8_t>& buffer, const Utils::Color& maskColor)
	{	
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

	template<typename T>
	mygal::Vector2<T> getClosestPointInMask(const mygal::Vector2<T>& point) const
	{
		auto x = static_cast<int>(point.x * width);
		auto y = static_cast<int>(point.y * height);

		if (isInMask(x, y))
		{
			return point;
		}

		std::unordered_set<mygal::Vector2<int>> pointsVisited;
		const  int startIdx = y * width + x;
		std::queue<mygal::Vector2<int>> queue;

		// Initialize BFS queue with the starting position
		queue.push({ x, y });

		while (!queue.empty())
		{
			auto [cx, cy] = queue.front();
			queue.pop();
			if (cx < 0 || cx >= width || cy < 0 || cy >= height) continue;
			if (pointsVisited.contains(mygal::Vector2<int>(cx, cy))) continue;


			if (data[cy * width + cx])
				return Utils::ScaleBothAxis(mygal::Vector2<T>(cx, cy), 1.0f/width, 1.0f/height);
			else
				pointsVisited.insert(mygal::Vector2<int>(cx, cy));

			queue.push({ cx + 1, cy });
			queue.push({ cx - 1, cy });
			queue.push({ cx, cy + 1 });
			queue.push({ cx, cy - 1 });

		}

		//// If not, perform a search for the nearest valid point.
		//double bestDist = std::numeric_limits<double>::max();
		//mygal::Vector2<double> bestPoint = Utils::ScaleBothAxis(mygal::Vector2<double>(point), width, height);

		//// You can use a simple square radius search around the point.
		//int maxRadius = std::max(width / 4, height /4);
		//for (int r = 1; r < maxRadius; ++r) 
		//{
		//	for (int dx = -r; dx <= r; ++dx) 
		//	{
		//		for (int dy = -r; dy <= r; ++dy) 
		//		{
		//			int x = bestX + dx;
		//			int y = bestY + dy;
		//			if (isInMask(x, y))
		//			{
		//				double dist = (x - point.x)(x - point.x) + (y - point.y)(y - point.y);
		//				if (dist < bestDist) 
		//				{
		//					bestDist = dist;
		//					bestPoint = { static_cast<double>(x), static_cast<double>(y) };
		//				}
		//			}
		//		}
		//	}
		//	if (bestDist != std::numeric_limits<double>::max()) {
		//		break; // Stop early if a valid point is found.
		//	}
		
		return point;
	}
};

}