#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <chrono>
#include <MyGAL/Vector2.h>
#include <MyGAL/Diagram.h>
#include <unordered_set>
#include <stack>
#include "Color.h"
#include "Rasterizer.h"
constexpr double PointRadius = 1.0f;
constexpr double Offset = 1.0f;
namespace MapGeneratorTool
{
namespace rend
{
    using namespace  mygal;
    template<typename T>
    void drawPolygons(const std::vector<std::vector<Vector2<T>>>& polygons, sf::RenderTexture& texture, unsigned width, unsigned height)
    {
        if (texture.getSize() == sf::Vector2u(0, 0))
        {
            if (!texture.create(width, height))
            {
                // error...
                std::cout << "Error creating texture to draw polygons to map" << "\n";
                return;
            }
        }

        std::unordered_set<Utils::Color> colorsInUse;
        // drawing uses the same functions
        texture.clear();
        for (const std::vector<Vector2<T>>& pol : polygons)
        {
            sf::ConvexShape convex;
            convex.setPointCount(pol.size());
            for (int i = 0; i < pol.size(); i++)
            {
                sf::Vector2f point(pol[i].x * width, pol[i].y * height);
                convex.setPoint(i, point);
            }
            Utils::Color color;
            do {
                color.RandColor();
            } while (colorsInUse.contains(color));

            colorsInUse.emplace(color);
            convex.setFillColor(sf::Color(color.R, color.G, color.B));
            texture.draw(convex);
            texture.display();
        }

        assert(colorsInUse.size() == polygons.size());
    }

    template<typename T>
    void drawPolygons(const std::vector<std::vector<Vector2<T>>>& polygons, sf::RenderTexture& texture, unsigned width, unsigned height, std::unordered_set<Utils::Color>& colorsInUse)
    {
        if (texture.getSize() == sf::Vector2u(0, 0))
        {
            if (!texture.create(width, height))
            {
                // error...
                std::cout << "Error creating texture to draw polygons to map" << "\n";
                return;
            }
        }

        // drawing uses the same functions
        texture.clear();
        for (const std::vector<Vector2<T>>& pol : polygons)
        {
            sf::ConvexShape convex;
            convex.setPointCount(pol.size());
            for (int i = 0; i < pol.size(); i++)
            {
                sf::Vector2f point(pol[i].x * width, pol[i].y * height);
                convex.setPoint(i, point);
            }
            Utils::Color color;
            do {
                color.RandColor();
            } while (colorsInUse.contains(color));

            colorsInUse.emplace(color);
            convex.setFillColor(sf::Color(color.R, color.G, color.B));
            texture.draw(convex);
            texture.display();
        }

        //assert(colorsInUse.size() == polygons.size());
    }

    // Function to calculate cross product of vectors (P1P2) and (P1P)
    template<typename T>
    float crossProduct(const Vector2<T>& P1, const Vector2<T>& P2, const Vector2<int>& P, unsigned width, unsigned height)
    {
        Vector2<T> p1 = P1;
        Vector2<T> p2 = P2;
        //const Vector2<T> p = P;
        p1.x *= width;
        p2.x *= width;
        //p3.x*=width;

        p1.y *= height;
        p2.y *= height;
        //p3.y *= height;

        return (p2.x - p1.x) * (P.y - p1.y) - (p2.y - p1.y) * (P.x - p1.x);
    }

    template<typename T>
    bool isPointInsidePolygon(const std::vector <Vector2<T>>& polygon, Vector2<int> point, unsigned width, unsigned height)
    {
        int n = polygon.size();
        if (n < 3) return false;  // A polygon must have at least 3 points

        bool allPositive = false;
        bool allNegative = false;

        for (int i = 0; i < n; ++i)
        {
            // Get the current vertex and the next vertex (loop back at the end)
            const auto& P1 = polygon[i];
            const auto& P2 = polygon[(i + 1) % n];

            // Calculate cross product
            float cross = crossProduct(P1, P2, point, width, height);
            if (i == 0)
            {
                // Initialize the sign of the first cross product
                if (cross > 0) allPositive = true;
                else if (cross < 0) allNegative = true;
            }
            else {
                // Ensure that all cross products have the same sign
                if (allPositive && cross < 0) return false;
                if (allNegative && cross > 0) return false;
            }
        }

        // If all cross products are positive or all are negative, the point is inside
        return true;
    }


    static void saveToFile(const sf::RenderTexture& texture, const char* filename)
    {
        texture.getTexture().copyToImage().saveToFile(filename);
    }

    static void drawBuffer(const std::vector<sf::Uint8>& buffer, sf::RenderTexture& renderTexture, unsigned width, unsigned height)
    {
        //auto start = std::chrono::steady_clock::now();
        if (renderTexture.getSize() == sf::Vector2u(0, 0))
        {
            if (!renderTexture.create(width, height))
            {
                // error...
                std::cout << "Error creating texture to draw polygons to map" << "\n";
                return;
            }
        }
        // Create a texture
        sf::Texture texture;
        if (!texture.create(width, height)) {
            return; // Error handling
        }

        // Update the texture with the pixel buffer
        texture.update(buffer.data());


        renderTexture.clear();
        sf::Sprite sprite(texture);
        renderTexture.draw(sprite);
        renderTexture.display();
        //auto duration = std::chrono::steady_clock::now() - start;
        //std::cout << "draw buffer: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
    }

    template<typename T>
    void drawPolygonsByBuffer(const std::vector<std::vector<Vector2<T>>>& polygons, sf::RenderTexture& texture, unsigned width, unsigned height, std::unordered_set<Utils::Color>& colorsInUse)
    {
        std::vector<uint8_t> buffer(width * height * 4);

        if (texture.getSize() == sf::Vector2u(0, 0))
        {
            if (!texture.create(width, height))
            {
                // error...
                std::cout << "Error creating texture to draw polygons to map" << "\n";
                return;
            }
        }

        // drawing uses the same functions
        texture.clear();
        for (const std::vector<Vector2<T>>& pol : polygons)
        {
            Utils::Color color;
            do {
                color.RandColor();
            } while (colorsInUse.contains(color));

            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    if (isPointInsidePolygon(pol, Vector2(x, y), width, height))
                    {
                        unsigned int index = (y * width + x) * 4;
                        colorsInUse.emplace(color);
                        buffer[index] = color.R;
                        buffer[index + 1] = color.G;
                        buffer[index + 2] = color.B;
                        buffer[index + 3] = color.A;

                        break;
                    }
                }

            }
        }

        drawBuffer(buffer, texture, width, height);
    }


    template<typename T>
    void drawPoint(sf::RenderTexture& window, Vector2<T> point, sf::Color color, unsigned width, unsigned height)
    {
        auto shape = sf::CircleShape(PointRadius);
        shape.setPosition(sf::Vector2f(point.x * width - PointRadius, point.y * height - PointRadius));
        shape.setFillColor(color);
        window.draw(shape);
        window.display();
    }

    template<typename T>
    void drawEdge(sf::RenderTexture& window, Vector2<T> origin, Vector2<T> destination, sf::Color color, unsigned width, unsigned height)
    {
        auto line = std::array<sf::Vertex, 2>
        {
            sf::Vertex(sf::Vector2f(origin.x* width, origin.y* height), color),
                sf::Vertex(sf::Vector2f(destination.x* width, destination.y* height), color)
        };
        window.draw(line.data(), 2, sf::Lines);
        window.display();
    }

    template<typename T>
    void drawPoints(sf::RenderTexture& window, const Diagram<T>& diagram, unsigned width, unsigned height)
    {
        for (const auto& site : diagram.getSites())
            drawPoint(window, site.point, sf::Color(255, 0, 0), width, height);
    }

    template<typename T>
    void drawDiagram(sf::RenderTexture& window, const Diagram<T>& diagram, unsigned width, unsigned height)
    {
        for (const auto& site : diagram.getSites())
        {
            auto center = site.point;
            auto face = site.face;
            auto halfEdge = face->outerComponent;
            if (halfEdge == nullptr)
                continue;
            while (halfEdge->prev != nullptr)
            {
                halfEdge = halfEdge->prev;
                if (halfEdge == face->outerComponent)
                    break;
            }
            auto start = halfEdge;
            while (halfEdge != nullptr)
            {
                if (halfEdge->origin != nullptr && halfEdge->destination != nullptr)
                {
                    auto origin = (halfEdge->origin->point - center) * Offset + center;
                    auto destination = (halfEdge->destination->point - center) * Offset + center;
                    drawEdge(window, origin, destination, sf::Color::Red, width, height);
                }
                halfEdge = halfEdge->next;
                if (halfEdge == start)
                    break;
            }
        }
    }

   /* static void drawPointsBuffer(sf::RenderTexture& rTex, const std::vector<delaunator::Point>& points, sf::Color color = sf::Color::Red)
    {
        for (auto pt : points)
        {
            auto shape = sf::CircleShape(PointRadius);
            shape.setPosition(sf::Vector2f(pt.x() - PointRadius, pt.y() - PointRadius));
            shape.setFillColor(color);
            rTex.draw(shape);
            rTex.display();
        }
    }*/


    static void drawTileMap(sf::RenderTexture& rTex, const std::vector<rasterizer::Tile>& tileMap, unsigned width, unsigned height)
    {
        //floodFill(tileMap, )
        std::vector<uint8_t> buffer = rasterizer::ConvertTileMapToBuffer(tileMap);
        drawBuffer(buffer, rTex, width, height);
    }


    static bool fill(int x, int y, std::vector<rasterizer::Tile>& tileMap, const Utils::Color& newColor, unsigned width, unsigned height)
    {
        const int index = y * width + x;
        const bool tileType = tileMap[index].land;

        
        if(tileMap[index].isBorder)
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

            if(tile.isBorder)
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
    


}
}