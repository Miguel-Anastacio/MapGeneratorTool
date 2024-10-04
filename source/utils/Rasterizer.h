#pragma once
#include <MyGAL/Vector2.h>
#include <vector>
#include <MyGAL/Diagram.h>
#include <geometry/ComputeGeometry.h>
namespace MapGeneratorTool
{

namespace rasterizer
{
	static void plotTile(int x, int y, unsigned width, unsigned height, std::vector<int>& tileMap)
	{
		if (x >= 0 && x < width && y >= 0 && y < height) 
		{
			int index = y * width + x;
			tileMap[index] = 1; // Mark the tile as visited
		}
	}

	

	template <typename T>
	static void Line(const mygal::Vector2<T>& start, const mygal::Vector2<T>& end, std::vector<int>& tileMap, unsigned width, unsigned height, float noiseScale)
	{
		// Create line
		T dx = (end.x - start.x) * width;
		T dy = (end.y - start.y) * height;
		T length = std::sqrt(dx * dx + dy * dy);  // Line length
		T stepSize = std::sqrt(2.0f) / 2.0f;      // Step size
		int numSteps = std::ceil(length / stepSize);  // Number of steps

		std::vector<mygal::Vector2<T>> line(numSteps + 1);

		for (int i = 0; i <= numSteps; ++i) 
		{
			T t = i / static_cast<float>(numSteps);
			T x = start.x * width + t * dx;
			T y = start.y * height + t * dy;
			line[i] = mygal::Vector2(x, y);
		}

		geomt::DeformLine(line, noiseScale);

		for (auto& point : line)
		{
			plotTile(static_cast<int>(point.x), static_cast<int>(point.y), width, height, tileMap); // Cast coordinates to integers and plot
		}
	}


	template <typename T>
	struct LineData
	{
		mygal::Vector2<T> start;
		mygal::Vector2<T> end;

		LineData(const mygal::Vector2<T>& s, const mygal::Vector2<T>& e) : start(s), end(e) {};

		bool operator==(const LineData& other) const 
		{
			return (start == other.start && end == other.end) || (start == other.end && end == other.start);
		}
	};

	template <typename T>
	static std::vector<int> CreateTileFromDiagram(const mygal::Diagram<T>& diagram, unsigned width, unsigned height, float noiseScale = 25.0f)
	{
		std::vector<int> tileMap(width * height, 0);
		//std:

		//for (const auto& site : diagram.getSites())
		//{
		//	mygal::Vector2 center = site.point;
		//	auto face = site.face;
		//	auto halfEdge = face->outerComponent;
		//	if (halfEdge == nullptr)
		//		continue;
		//	while (halfEdge->prev != nullptr)
		//	{
		//		halfEdge = halfEdge->prev;
		//		if (halfEdge == face->outerComponent)
		//			break;
		//	}
		//	auto start = halfEdge;
		//	while (halfEdge != nullptr)
		//	{
		//		if (halfEdge->origin != nullptr && halfEdge->destination != nullptr)
		//		{
		//			auto origin = halfEdge->origin->point;
		//			auto destination = (halfEdge->destination->point;
		//			Line(origin, destination, tileMap, width, height, noiseScale);
		//		}
		//	/*	else if (halfEdge->origin != nullptr && halfEdge->destination == nullptr)
		//		{
		//			auto origin = (halfEdge->origin->point - center) + center;
		//			auto destination = (halfEdge->start->point - center) + center;
		//		}*/
		//		halfEdge = halfEdge->next;
		//		if (halfEdge == start)
		//			break;
		//	}
		//}

		std::unordered_set<LineData<T>> linesPlotted;

		for (const auto& halfEdge : diagram.getHalfEdges())
		{
			auto* start = &halfEdge;
			auto* it = &halfEdge;
			while (it != nullptr)
			{
				if (it->origin != nullptr && it->destination != nullptr)
				{
					auto origin = it->origin->point;
					auto destination = it->destination->point;

					if (!linesPlotted.contains(LineData(origin, destination)))
					{
						Line(origin, destination, tileMap, width, height, noiseScale);
						linesPlotted.insert(LineData(origin, destination));
					}


				}
				it = it->next;
				if (it == start)
					break;
			}
		}


		return tileMap;
	}
}
}


namespace std 
{
	template <typename T>
	struct hash<mygal::Vector2<T>> {
		size_t operator()(const mygal::Vector2<T>& vec) const {
			// Combine the hashes of x and y
			return std::hash<T>()(vec.x) ^ (std::hash<T>()(vec.y) << 1);
		}
	};
}

namespace std 
{
	template <typename T>
	struct hash<MapGeneratorTool::rasterizer::LineData<T>>
	{
		std::size_t operator()(const MapGeneratorTool::rasterizer::LineData<T>& line) const
		{
			size_t startHash = std::hash<mygal::Vector2<T>>()(line.start);
			size_t endHash = std::hash<mygal::Vector2<T>>()(line.end);

			// Combine hashes such that the order doesn't matter (e.g., addition or min/max approach)
			return startHash ^ endHash;
		}
	};
}
