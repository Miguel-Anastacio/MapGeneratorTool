#include "TileMap.h"
#include "Renderer.h"
namespace MapGeneratorTool
{
	TileMap::TileMap(unsigned width, unsigned height) : Dimensions(width, height)
	{
		m_tiles.reserve(width * height);
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
} // namespace MapGeneratorTool