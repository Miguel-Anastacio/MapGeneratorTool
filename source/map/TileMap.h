#pragma once
#include "Utils.h"
#include "Color.h"
#include "MyGAL/Vector2.h"
#include <vector>
#include <unordered_set>
#include "VectorWrapper.h"
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
class Mask;
class TileMap : public Utils::Dimensions
{

public:
	TileMap(unsigned width, unsigned height);

	void MarkTilesNotInMaskAsVisited(const Mask& mask, TileType type);

	void FloodFillTileMap(const std::vector<mygal::Vector2<double>>& centroids);

	void FloodFillMissingTiles(int radius = 100);

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

	inline void ComputeCentroids();

	void ComputeColorsInUse();
	std::unordered_set <Utils::Color> GetColors()
	{
		return  m_colors;
	}

	std::unordered_set<mygal::Vector2<int>> GetCentroids() const
	{
		return m_centroids;
	}

	size_t GetColorsInUse() const;

	bool IsTileOfType(TileType type, int x, int y) const
	{
		if (x > Width() || y > Height())
		{
			return false;
		}

		return m_tiles[y * Width() + x].type == type;
	}

	static TileMap BlendTileMap(const TileMap& tileMap1, TileType type1, const TileMap& tileMap2, TileType type2);

	void InsertCentroid(const mygal::Vector2<int>& point, const Utils::Color& color);

	bool FindColorOfClosestTileOfSameType(int x, int y, int radius, Utils::Color& out_color) const;

private:
	std::vector<Tile> m_tiles;
	std::unordered_set<mygal::Vector2<int>> m_centroids;
	std::unordered_set<Utils::Color> m_colors;


};


} // namespace MapGeneratorTool

