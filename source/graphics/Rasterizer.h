#pragma once
#include <vector>
#include <MyGAL/Diagram.h>
#include <geometry/ComputeGeometry.h>
#include "Color.h"
#include "TileMap.h"
#include "MapSpecs.h"
#include "VectorWrapper.h"
namespace MapGeneratorTool
{
	namespace rasterizer
	{

		void plotTile(int x, int y, unsigned width, unsigned height, std::vector<Tile>& tileMap, const mygal::Vector2<int>& centroid);

		template <typename T>
		static void Line(const mygal::Vector2<T>& start, const mygal::Vector2<T>& end, std::vector<Tile>& tileMap, unsigned width, unsigned height,
			FastNoiseLite& noise, float noiseScale, T thick, const mygal::Vector2<int>& centroid)
		{
			// Create line
			T dx = (end.x - start.x);
			T dy = (end.y - start.y);
			T length = std::sqrt(dx * dx + dy * dy);  // Line length
			T stepSize = std::sqrt(2.0f) / 2.0f;      // Step size
			int numSteps = static_cast<int>(std::ceil(length / stepSize));  // Number of steps

			std::vector<mygal::Vector2<T>> line(numSteps + 1);

			for (int i = 0; i <= numSteps; ++i)
			{
				T t = i / static_cast<float>(numSteps);
				T x = start.x + t * dx;
				T y = start.y + t * dy;
				line[i] = mygal::Vector2(x, y);
			}

			geomt::DeformLine(line, noise, noiseScale);

			mygal::Vector2<T> tangent = end - start;
			for (size_t i = 0; i < line.size(); i++)
			{
				const auto& point = line[i];
				if (i + 1 < line.size())
				{
					mygal::Vector2<T> tangent = line[i+1] - point;
				}

				auto perpendicular = mygal::normed(tangent.getOrthogonal());
				if (std::abs(thick - 1) < 0.01)
				{
					plotTile(static_cast<int>(point.x), static_cast<int>(point.y), width, height, tileMap, centroid);
				}
				else
				{
					mygal::Vector2<double> offsetUp = point + perpendicular * (thick / 2);
					mygal::Vector2<double> offsetDown = point - perpendicular * (thick / 2);

					T dTx = (offsetDown.x - offsetUp.x);
					T dTy = (offsetDown.y - offsetUp.y);

					int steps = static_cast<int>(std::ceil(offsetUp.getDistance(offsetDown) / stepSize));
					for (int s = 0; s <= steps; ++s)
					{
						T t = s / static_cast<T>(steps);
						T x = offsetUp.x + t * dTx;
						T y = offsetUp.y + t * dTy;
						plotTile(static_cast<int>(x), static_cast<int>(y), width, height, tileMap, centroid);
					}
					
				}

			}
		}

		//template <typename T>
		//static std::array<const mygal::Vector2<T>, 4> createParallelogram(const mygal::Vector2<T> p1, const mygal::Vector2<T> p2, float width)
		//{
		//	// Compute the direction vector of the line
		//	float dx = p2.x - p1.x;
		//	float dy = p2.y - p1.y;

		//	// Compute the normal vector to the line
		//	float length = std::sqrt(dx * dx + dy * dy);
		//	float nx = -(dy / length) * (width / 2);
		//	float ny = (dx / length) * (width / 2);

		//	// Define the four corners of the parallelogram
		//	const mygal::Vector2<T> p1Prime = { p1.x + nx, p1.y + ny }; // Top-left corner
		//	const mygal::Vector2<T> p1DoublePrime = { p1.x - nx, p1.y - ny }; // Bottom-left corner
		//	const mygal::Vector2<T> p2Prime = { p2.x + nx, p2.y + ny }; // Top-right corner
		//	const mygal::Vector2<T> p2DoublePrime = { p2.x - nx, p2.y - ny }; // Bottom-right corner

		//	return { p1Prime, p1DoublePrime, p2Prime, p2DoublePrime };
		//}

		//template <typename T>
		//static void rasterizeParallelogram(const mygal::Vector2<T> p1Prime, const mygal::Vector2<T> p1DoublePrime, const mygal::Vector2<T> p2Prime, const mygal::Vector2<T> p2DoublePrime,
		//	std::vector<Tile>& tileMap, unsigned width, unsigned height, float noiseScale, FastNoiseLite& noise)
		//{
		//	// Rasterize two triangles
		//	Line(p1Prime, p2Prime, tileMap, width, height, noise, noiseScale); // Top line
		//	Line(p1DoublePrime, p2DoublePrime, tileMap, width, height, noise, noiseScale); // Bottom line

		//	// Fill in the interior of the parallelogram (simple naive horizontal stepping)
		//	float stepSize = std::sqrt(2.0f) / 2.0f;
		//	for (float t = 0; t <= 1.0f; t += stepSize)
		//	{
		//		mygal::Vector2<T> top(p1Prime.x + t * (p2Prime.x - p1Prime.x), p1Prime.y + t * (p2Prime.y - p1Prime.y));
		//		mygal::Vector2<T> bottom(p1DoublePrime.x + t * (p2DoublePrime.x - p1DoublePrime.x), p1DoublePrime.y + t * (p2DoublePrime.y - p1DoublePrime.y));


		//		Line(top, bottom, tileMap, width, height, noise, noiseScale);
		//	}
		//}


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
	}
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

namespace MapGeneratorTool
{
namespace rasterizer
{
	template <typename T>
	static void RasterizeDiagramToTileMap(const mygal::Diagram<T>& diagram, int width, int height, TileMap& tileMap, const NoiseData& noiseData, T borderThick)
	{
		std::vector<Tile>& tileMapVector = tileMap.GetTilesRef();
		std::unordered_set<LineData<T>> linesPlotted;
		 
		for (const auto& halfEdge : diagram.getHalfEdges())
		{
			mygal::Vector2<T> centroidRaw = halfEdge.incidentFace->site->point;
			centroidRaw = mygal::ScaleBothAxis(centroidRaw, width, height);
			mygal::Vector2<int> centroid = mygal::Vector2<int>(centroidRaw.x, centroidRaw.y);

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
						const mygal::Vector2<double> start = mygal::ScaleBothAxis(origin, static_cast<double>(width), static_cast<double>(height));
						const mygal::Vector2<double> end = mygal::ScaleBothAxis(destination, static_cast<double>(width), static_cast<double>(height));
						
						FastNoiseLite noise(noiseData.seed);
						ApplyNoiseData(noiseData, noise);
						//auto parallelogram = createParallelogram(start, end, borderThick);
						//rasterizeParallelogram(parallelogram[0], parallelogram[1], parallelogram[2], parallelogram[3], 
							//tileMapVector, width, height, noiseData.scale, noise);
						Line(start, end, tileMapVector, width, height, noise, noiseData.scale, borderThick, centroid);
						//plotLineWithWidth(start, end, tileMapVector, width, height, noise, noiseData.scale, borderThick);


						linesPlotted.insert(LineData(origin,destination));
					}
				}
				it = it->next;
				if (it == start)
					break;
			}
		}
		//return tileMap;
	}
}
}

