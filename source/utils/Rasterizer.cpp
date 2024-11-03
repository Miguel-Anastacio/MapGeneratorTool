#pragma once
#include <Rasterizer.h>
namespace MapGeneratorTool
{
namespace rasterizer
{
void plotTile(int x, int y, unsigned width, unsigned height, std::vector<Tile>& tileMap, const mygal::Vector2<int>& centroid)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		int index = y * width + x;
		tileMap[index].visited = true; // Mark the tile as visited
		tileMap[index].isBorder = true; // Mark the tile as visited
		tileMap[index].centroid = centroid;
	}
}

} // namespace MapGeneratorTool
} // namespace rasterizer
