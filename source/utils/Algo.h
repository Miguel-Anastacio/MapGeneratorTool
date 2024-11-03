#pragma once
#include <vector>
#include <Rasterizer.h>
#include <stack>
namespace MapGeneratorTool
{

namespace algo
{ 
    
static bool fill(int x, int y, std::vector<Tile>& tileMap, const Utils::Color& newColor, unsigned width, unsigned height)
{
    const int index = y * width + x;
    const auto tileType = tileMap[index].type;
    auto wid = static_cast<int>(width);
    auto hgt = static_cast<int>(height);

    if (x >= wid || x < 0 || y >= hgt || y < 0 || tileMap[index].color == newColor || tileMap[index].visited)
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
        if (cx < 0 || cx >= wid || cy < 0 || cy >= hgt) continue;
        Tile& tile = tileMap[cy * wid + cx];

        //if (tile.isBorder && tile.type == tileType) 
        //    tile.centroid = mygal::Vector2<int>(x, y);

        if (tile.visited || tile.color == newColor) continue;

        if (tile.type != tileType) continue;

        // Mark tile as visited and set its color
        tile.color = newColor;
        tile.visited = true;
        //tile.centroid = mygal::Vector2<int>(x, y);

        // Push neighboring tiles onto the stack
        stack.push({ cx + 1, cy });
        stack.push({ cx - 1, cy });
        stack.push({ cx, cy + 1 });
        stack.push({ cx, cy - 1 });
    }

    return true;

}

static mygal::Vector2<int> fillGetCentroidOfPoints(int x, int y, std::vector<Tile>& tileMap, const Utils::Color& newColor, unsigned width, unsigned height)
{
    auto wid = static_cast<int>(width);
    auto hgt = static_cast<int>(height);

    const int index = y * width + x;
    const auto tileType = tileMap[index].type;
    mygal::Vector2<int> pointsSum;
    std::vector<Tile*> tilesRef;
    int count = 0;


    if (x >= wid || x < 0 || y >= hgt || y < 0 || tileMap[index].color == newColor || tileMap[index].visited)
    {
        return mygal::Vector2<int>(-1, -1);
    }
    // Stack for iterative approach
    std::stack<std::pair<int, int>> stack;
    stack.push({ x, y });

    while (!stack.empty())
    {
        auto [cx, cy] = stack.top();
        stack.pop();

        // Check boundary conditions again and ensure tile hasn't been visited
        if (cx < 0 || cx >= wid || cy < 0 || cy >= hgt) continue;
        Tile& tile = tileMap[cy * wid + cx];

        //if (tile.isBorder && tile.type == tileType) 
        //    tile.centroid = mygal::Vector2<int>(x, y);

        if (tile.visited || tile.color == newColor) continue;

        if (tile.type != tileType) continue;

        // Mark tile as visited and set its color
        tile.color = newColor;
        tile.visited = true;
        //tile.centroid = mygal::Vector2<int>(x, y);
        pointsSum += mygal::Vector2(cx, cy);
        count++;

        // Push neighboring tiles onto the stack
        stack.push({ cx + 1, cy });
        stack.push({ cx - 1, cy });
        stack.push({ cx, cy + 1 });
        stack.push({ cx, cy - 1 });
        tilesRef.emplace_back(&tile);
    }

    const mygal::Vector2<int> centroid = pointsSum / count;
    for (auto& tile : tilesRef)
    {
        tile->centroid = centroid;
    }

    return centroid;

}

static void markCentroids(int centrooidX, int centrooidY, std::vector<Tile>& tileMap, const Utils::Color& newColor, unsigned width, unsigned height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            auto index = y * width + x;
            if (tileMap[index].color == newColor )
            {
                tileMap[index].centroid = mygal::Vector2(centrooidX, centrooidY);
            }
            
        }
    }
}

static void floodFill(std::vector<Tile>& tileMap, const std::vector<mygal::Vector2<double>>& centroids, int width, int height)
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


        x = std::clamp(x, 0, width - 1);
        y = std::clamp(y, 0, height - 1);

        if (fill(x, y, tileMap, color, width, height))
            colorsInUse.insert(color);
        markCentroids(x, y, tileMap, color, width, height);


    }
}
} // namespace algo
}// namespace MapGeneratorTool