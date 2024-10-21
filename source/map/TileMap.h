#pragma once
#include "Utils.h"
#include "Color.h"
#include "MyGAL/Vector2.h"
#include <vector>
namespace MapGeneratorTool
{
enum class TileType : uint8_t
{
	UNDEFINED = 0,
	LAND = 1,
	WATER = 2
};

struct Tile
{
	Utils::Color color;
	bool visited;
	TileType type;
	bool isBorder;
	mygal::Vector2<int> centroid;

	Tile(bool state, const Utils::Color& col, TileType typ, const mygal::Vector2<int>& centroid) : color(col), visited(state), type(typ), isBorder(false) {}
	Tile() : color(Utils::Color(0, 0, 0, 0)), visited(false), type(TileType::UNDEFINED), isBorder(false), centroid(0, 0) {}

};

class TileMap : public Utils::Dimensions
{

public:
	TileMap(unsigned width, unsigned height);

	inline Tile GetTile(unsigned x, unsigned y) const
	{
		if (x > Width() || y > Height())
		{

		}

		return m_tiles[y * Width() + x];
	}

	std::vector<uint8_t> ConvertTileMapToBuffer() const;
	inline std::vector<Tile>& GetTilesRef()
	{
		return m_tiles;
	}

	inline std::vector<Tile> GetTiles() const
	{
		return m_tiles;
	}

	static TileMap BlendTileMap(const TileMap& tileMap1, TileType type1, const TileMap& tileMap2, TileType type2);
private:
	std::vector<Tile> m_tiles;


};


} // namespace MapGeneratorTool

