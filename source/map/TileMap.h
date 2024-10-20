#pragma once
#include "Utils.h"
#include "Color.h"
#include "MyGAL/Vector2.h"
#include <vector>
namespace MapGeneratorTool
{
enum class TileType : uint8_t
{
	LAND = 0,
	WATER = 1
};

struct Tile
{
	Utils::Color color;
	bool visited;
	bool land;
	bool isBorder;
	mygal::Vector2<int> centroid;

	Tile(bool state, const Utils::Color& col, bool l, const mygal::Vector2<int>& centroid) : color(col), visited(state), land(l), isBorder(false) {}
	Tile() : color(Utils::Color(0, 0, 0, 0)), visited(false), land(true), isBorder(false), centroid(0, 0) {}

};

class TileMap : public Utils::Dimensions
{

public:
	TileMap(unsigned width, unsigned height);


	std::vector<uint8_t> ConvertTileMapToBuffer() const;

private:
	std::vector<Tile> m_tiles;
};

} // namespace MapGeneratorTool

