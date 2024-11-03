#include "TileMap.h"
#include "Renderer.h"
#include <cmath>
#include "Algo.h"
#include "Mask.h"
#include "Timer.h"
namespace MapGeneratorTool
{
	TileMap::TileMap(unsigned width, unsigned height) : Dimensions(width, height), m_tiles(width*height, Tile())
	{

	}
	void TileMap::MarkTilesNotInMaskAsVisited(const Mask& mask, TileType type)
	{

		auto width = Width();
		auto height = Height();
		Timer timer;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (!mask.isInMask(x, y))
				{
					m_tiles[y * width + x].visited = true;
				}
				else
				{
					m_tiles[y * width + x].type = type;
				}
			}
		}
		std::cout << "Time for MarkTilesNotInMaskAsVisited (msec): " << timer.elapsedMilliseconds() << "\n";
	}
	void TileMap::FloodFillTileMap(const std::vector<mygal::Vector2<double>>& centroids)
	{
		auto width = Width();
		auto height = Height();
		Timer timer;
		algo::floodFill(m_tiles, centroids, width, height);
		std::cout << "Time for FloodFillTileMap (msec): " << timer.elapsedMilliseconds() << "\n";

		ComputeCentroids();
		ComputeColorsInUse();
	}

	void TileMap::FloodFillMissingTiles(int radius)
	{
		auto width = Width();
		auto height = Height();
		Timer timer;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto index = y * width + x;
				if (!m_tiles[index].visited)
				{
					Utils::Color color;
					if (FindColorOfClosestTileOfSameType(x, y, radius, color))
					{
						algo::fill(x, y, m_tiles, color, width, height);
					}
					else
					{
						do {
							color.RandColor();
						} while (m_colors.contains(color));

						mygal::Vector2<int> point = algo::fillGetCentroidOfPoints(x, y, m_tiles, color, width, height);
						if (point != mygal::Vector2(-1, -1))
							InsertCentroid(point, color);

					}
				}
			}
		}
		std::cout << "Time for FloodFillMissingTiles (msec): " << timer.elapsedMilliseconds() << "\n";
	}
	std::vector<uint8_t> TileMap::ConvertTileMapToBuffer() const
	{
		std::vector<uint8_t> buffer;
		buffer.reserve(m_tiles.size() * 4);
		for (auto& tile : m_tiles)
		{
			buffer.emplace_back(tile.color.R);
			buffer.emplace_back(tile.color.G);
			buffer.emplace_back(tile.color.B);
			buffer.emplace_back(tile.color.A);
		}

		return buffer;
	}

	inline void TileMap::ComputeCentroids()
	{
		auto height = Height();
		auto width = Width();
		m_centroids.clear();

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				const int index = y * width + x;
				m_centroids.emplace(m_tiles[index].centroid);
			}
		}
	}

	void TileMap::ComputeColorsInUse()
	{
		auto height = Height();
		auto width = Width();
		m_colors.clear();
		m_colors.reserve(m_centroids.size());

		for (unsigned y = 0; y < height; y++)
		{
			for (unsigned x = 0; x < width; x++)
			{
				const unsigned index = y * width + x;
				m_colors.emplace(m_tiles[index].color);
			}
		}
	}

	size_t TileMap::GetColorsInUse() const
	{
		return m_colors.size();
	}

	TileMap TileMap::BlendTileMap(const TileMap& tileMap1, TileType type1, const TileMap& tileMap2, TileType type2)
	{
		assert(tileMap1.Height() == tileMap2.Height());
		assert(tileMap1.Width() == tileMap2.Width());

		auto height = tileMap1.Height();
		auto width = tileMap1.Width();
		Timer timer;

		TileMap newTileMap(width, height);
		auto& newTiles = newTileMap.GetTilesRef();

		auto tiles1 = tileMap1.GetTiles();
		auto tiles2 = tileMap2.GetTiles();

		for (unsigned y = 0; y < height; y++)
		{
			for (unsigned x = 0; x < width; x++)
			{
				const unsigned index = y * width + x;
				if (tiles1[index].type == type1)
				{
					newTiles[index] = tiles1[index];
				}
				else if (tiles2[index].type == type2)
				{
					newTiles[index] = tiles2[index];

				}
			}
		}
		std::cout << "Time for BlendTileMap (msec): " << timer.elapsedMilliseconds() << "\n";
		newTileMap.ComputeCentroids();

		return newTileMap;
	}

	void TileMap::InsertCentroid(const mygal::Vector2<int>& point, const Utils::Color& color)
	{
		m_colors.insert(color);
		m_centroids.insert(point);
	}

	bool TileMap::FindColorOfClosestTileOfSameType(int x, int y, int radius, Utils::Color& out_color) const
	{
		auto width = Width();
		auto height = Height();
		std::unordered_set<mygal::Vector2<int>> tilesVisited;
		const  int startIdx = y * Width() + x;
		auto targetType = m_tiles[startIdx].type;
		std::queue<std::pair<int, int>> queue;

		// Initialize BFS queue with the starting position
		queue.push({ x, y });

		// Directions array for exploring neighboring tiles: right, left, down, up

		while (!queue.empty())
		{
			auto [cx, cy] = queue.front();
			queue.pop();
			if (cx < 0 || cx >= width || cy < 0 || cy >= height) continue;
			if (tilesVisited.contains(mygal::Vector2<int>(cx, cy))) continue;

			const Tile& tile = m_tiles[cy * width + cx];

			if (tile.type == targetType && (tile.visited) && (!tile.isBorder))
			{
				out_color = tile.color;
				return true;
			}
			else
			{
				tilesVisited.insert(mygal::Vector2<int>(cx, cy));
			}

			// did not find tile in radius 
			if (std::abs(x + radius) < std::abs(cx) || std::abs(y + radius) < std::abs(cy))
			{
				return false;
			}

			queue.push({ cx + 1, cy });
			queue.push({ cx - 1, cy });
			queue.push({ cx, cy + 1 });
			queue.push({ cx, cy - 1 });

		}

		return false;
	}
} // namespace MapGeneratorTool