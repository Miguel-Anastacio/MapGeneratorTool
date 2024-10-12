#pragma once
#include <MyGAL/Vector2.h>
#include <vector>
#include <MyGAL/Diagram.h>
#include <geometry/ComputeGeometry.h>
#include "Color.h"
namespace MapGeneratorTool
{

namespace rasterizer
{
	struct Tile
	{
		Utils::Color color;
		bool visited;
		bool land;
		bool isBorder;
		mygal::Vector2<int> centroid;
			
		Tile(bool state, const Utils::Color& col, bool l, const mygal::Vector2<int>& centroid) : color(col), visited(state), land(l), isBorder(false){}
		Tile() : color(Utils::Color(0, 0, 0, 0)), visited(false), land(true), isBorder(false), centroid(0, 0) {}


	};

	static std::vector<uint8_t> ConvertTileMapToBuffer(const std::vector<Tile>& tileMap) 
	{
		std::vector<uint8_t> buffer;
		buffer.reserve(tileMap.size() * 4);
		for(auto& tile: tileMap)
		{
			buffer.emplace_back(tile.color.R);
			buffer.emplace_back(tile.color.G);
			buffer.emplace_back(tile.color.B);
			buffer.emplace_back(tile.color.A);
		}

		return buffer;
	}
	static void plotTile(int x, int y, unsigned width, unsigned height, std::vector<Tile>& tileMap)
	{
		if (x >= 0 && x < width && y >= 0 && y < height) 
		{
			int index = y * width + x;
			tileMap[index].visited = true; // Mark the tile as visited
			tileMap[index].isBorder = true; // Mark the tile as visited

		}
	}

	template <typename T>
	static void Line(const mygal::Vector2<T>& start, const mygal::Vector2<T>& end, std::vector<Tile>& tileMap, unsigned width, unsigned height, 
		FastNoiseLite& noise, float noiseScale)
	{
		// Create line
		T dx = (end.x - start.x);
		T dy = (end.y - start.y);
		T length = std::sqrt(dx * dx + dy * dy);  // Line length
		T stepSize = std::sqrt(2.0f) / 2.0f;      // Step size
		int numSteps = std::ceil(length / stepSize);  // Number of steps

		std::vector<mygal::Vector2<T>> line(numSteps + 1);

		for (int i = 0; i <= numSteps; ++i) 
		{
			T t = i / static_cast<float>(numSteps);
			T x = start.x + t * dx;
			T y = start.y + t * dy;
			line[i] = mygal::Vector2(x, y);
		}

		geomt::DeformLine(line, noise, noiseScale);

		for (auto& point : line)
		{
			plotTile(static_cast<int>(point.x), static_cast<int>(point.y), width, height, tileMap); // Cast coordinates to integers and plot
		}
	}

	// Function to compute the normal vector for a given line
	template <typename T>
	static std::array<const mygal::Vector2<T>, 4> createParallelogram(const mygal::Vector2<T> p1, const mygal::Vector2<T> p2, float width)
	{
		// Compute the direction vector of the line
		float dx = p2.x - p1.x;
		float dy = p2.y - p1.y;

		// Compute the normal vector to the line
		float length = std::sqrt(dx * dx + dy * dy);
		float nx = -(dy / length) * (width / 2);
		float ny = (dx / length) * (width / 2);

		// Define the four corners of the parallelogram
		const mygal::Vector2<T> p1Prime = { p1.x + nx, p1.y + ny }; // Top-left corner
		const mygal::Vector2<T> p1DoublePrime = { p1.x - nx, p1.y - ny }; // Bottom-left corner
		const mygal::Vector2<T> p2Prime = { p2.x + nx, p2.y + ny }; // Top-right corner
		const mygal::Vector2<T> p2DoublePrime = { p2.x - nx, p2.y - ny }; // Bottom-right corner

		return { p1Prime, p1DoublePrime, p2Prime, p2DoublePrime };
	}

	template <typename T>
	static void rasterizeParallelogram(const mygal::Vector2<T> p1Prime, const mygal::Vector2<T> p1DoublePrime, const mygal::Vector2<T> p2Prime, const mygal::Vector2<T> p2DoublePrime, 
		std::vector<Tile>& tileMap, unsigned width, unsigned height, float noiseScale, FastNoiseLite& noise)
	{
		// Rasterize two triangles
		Line(p1Prime, p2Prime, tileMap, width, height, noise, noiseScale); // Top line
		Line(p1DoublePrime, p2DoublePrime, tileMap, width, height, noise, noiseScale); // Bottom line

		// Fill in the interior of the parallelogram (simple naive horizontal stepping)
		float stepSize = std::sqrt(2.0f) / 2.0f;
		for (float t = 0; t <= 1.0f; t += stepSize) 
		{
			mygal::Vector2<T> top (p1Prime.x + t * (p2Prime.x - p1Prime.x), p1Prime.y + t * (p2Prime.y - p1Prime.y));
			mygal::Vector2<T> bottom (p1DoublePrime.x + t * (p2DoublePrime.x - p1DoublePrime.x), p1DoublePrime.y + t * (p2DoublePrime.y - p1DoublePrime.y));


			Line(top, bottom, tileMap, width, height, noise,noiseScale);
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
	static std::vector<Tile> CreateTileFromDiagram(const mygal::Diagram<T>& diagram, unsigned width, unsigned height, float noiseScale = 25.0f)
	{
		std::vector<Tile> tileMap(width * height, Tile());
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
						auto start = origin;
						auto end = destination;
						start.x *= width;
						start.y *= height;
						end.x *= width;
						end.y *= height;
						FastNoiseLite noise(std::rand());
						//auto parallelogram = createParallelogram(start, end, 3.0f);
						//rasterizeParallelogram(parallelogram[0], parallelogram[1], parallelogram[2], parallelogram[3], tileMap, width, height, noiseScale, noise);
						Line(start, end, tileMap, width, height, noise, noiseScale);
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
