#pragma once
#include <vector>
#include <Rasterizer.h>
#include <stack>
namespace MapGeneratorTool
{

namespace algo
{ 
    
static bool fill(int x, int y, std::vector<rasterizer::Tile>& tileMap, const Utils::Color& newColor, unsigned width, unsigned height)
{
    const int index = y * width + x;
    const bool tileType = tileMap[index].land;


    if (tileMap[index].isBorder)
        tileMap[index].centroid = mygal::Vector2<int>(x, y);

    if (x >= width || x < 0 || y >= height || y < 0 || tileMap[index].color == newColor || tileMap[index].visited)
    {
        return false;
    }

    // Stack for iterative approach
    std::stack<std::pair<int, int>> stack;
    stack.push({ x, y });

    while (!stack.empty())
    {
        auto [cx, cy] = stack.top();
        stack.pop();

        // Check boundary conditions again and ensure tile hasn't been visited
        if (cx < 0 || cx >= width || cy < 0 || cy >= height) continue;
        rasterizer::Tile& tile = tileMap[cy * width + cx];

        if (tile.isBorder)
            tile.centroid = mygal::Vector2<int>(x, y);

        if (tile.visited || tile.color == newColor) continue;

        if (tile.land != tileType) continue;

        // Mark tile as visited and set its color
        tile.color = newColor;
        tile.visited = true;
        tile.centroid = mygal::Vector2<int>(x, y);

        // Push neighboring tiles onto the stack
        stack.push({ cx + 1, cy });
        stack.push({ cx - 1, cy });
        stack.push({ cx, cy + 1 });
        stack.push({ cx, cy - 1 });
    }

    return true;

}

static void floodFill(std::vector<rasterizer::Tile>& tileMap, std::vector<mygal::Vector2<double>>& centroids, unsigned width, unsigned height)
{
    std::vector<uint8_t> buffer(tileMap.size() * 4);
    std::unordered_set<Utils::Color> colorsInUse;

    for (auto center : centroids)
    {
        auto x = static_cast<int>(center.x * width);
        auto y = static_cast<int>(center.y * height);
        Utils::Color color;
        do {
            color.RandColor();
        } while (colorsInUse.contains(color));

        if (fill(x, y, tileMap, color, width, height))
            colorsInUse.insert(color);

    }
}
} // namespace algo
}// namespace MapGeneratorTool