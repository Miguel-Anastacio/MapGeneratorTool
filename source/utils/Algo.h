#pragma once
#include <vector>
#include "TileMap.h"
#include "Color.h"
#include "VectorWrapper.h"
#include <stack>
namespace MapGeneratorTool
{
namespace algo
{ 
    
bool fill(int x, int y, std::vector<Tile>& tileMap, const Utils::Color& newColor, unsigned width, unsigned height);

mygal::Vector2<int> fillGetCentroidOfPoints(int x, int y, std::vector<Tile>& tileMap,
                                                   const Utils::Color& newColor, unsigned width, unsigned height);

void markCentroids(int centrooidX, int centrooidY, std::vector<Tile>& tileMap, const Utils::Color& newColor, unsigned width, unsigned height);

void floodFill(std::vector<Tile>& tileMap, const std::vector<mygal::Vector2<double>>& centroids, 
               std::unordered_set<Utils::Color>& colorsInUse,
               int width, int height);

} // namespace algo
}// namespace MapGeneratorTool