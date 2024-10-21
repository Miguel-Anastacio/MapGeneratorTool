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
class TileMap;
namespace rend
{
    void saveToFile(const sf::RenderTexture& texture, const char* filename);
    void drawBuffer(const std::vector<sf::Uint8>& buffer, sf::RenderTexture& renderTexture, unsigned width, unsigned height);
    
    void drawTileMap(const TileMap& tileMap, sf::RenderTexture& renderTexture, unsigned width, unsigned height);

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
}
}