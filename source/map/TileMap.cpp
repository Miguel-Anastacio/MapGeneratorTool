#include "TileMap.h"
#include "Renderer.h"
namespace MapGeneratorTool
{
	TileMap::TileMap(unsigned width, unsigned height) : Dimensions(width, height), m_tiles(width*height, Tile())
	{

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

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				const int index = y * width + x;
				m_centroids.emplace(m_tiles[index].centroid);
			}
		}
	}

	size_t TileMap::GetColorsInUse() const
	{
		std::unordered_set<Utils::Color> colors;
		colors.reserve(m_centroids.size());

	/*	for (auto& point : m_centroids)
		{
			colors.emplace_back(GetTile(point.x, point.y).color);
		}*/
		for (int y = 0; y < Height(); y++)
		{
			for (int x = 0; x < Width(); x++)
			{
				const int index = y * Width() + x;
				colors.emplace(m_tiles[index].color);
			}
		}

		return colors.size();
	}

	TileMap TileMap::BlendTileMap(const TileMap& tileMap1, TileType type1, const TileMap& tileMap2, TileType type2)
	{
		assert(tileMap1.Height() == tileMap2.Height());
		assert(tileMap1.Width() == tileMap2.Width());

		auto height = tileMap1.Height();
		auto width = tileMap1.Width();

		TileMap newTileMap(width, height);
		auto& newTiles = newTileMap.GetTilesRef();

		auto tiles1 = tileMap1.GetTiles();
		auto tiles2 = tileMap2.GetTiles();

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				const int index = y * width + x;
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
		newTileMap.ComputeCentroids();

		return newTileMap;
	}
} // namespace MapGeneratorTool