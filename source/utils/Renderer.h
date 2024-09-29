#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <chrono>
#include <MyGAL/Vector2.h>
#include <MyGAL/Diagram.h>
#include <unordered_set>
#include "Color.h"
#include <delaunator/include/delaunator.hpp>
constexpr double PointRadius = 0.1f;
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
            return ; // Error handling
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
    void drawPoint(sf::RenderTexture& window, Vector2<T> point, sf::Color color, unsigned width, unsigned height)
    {
        auto shape = sf::CircleShape(PointRadius);
        shape.setPosition(sf::Vector2f(point.x*width - PointRadius , point.y*height - PointRadius));
        shape.setFillColor(color);
        window.draw(shape);
        window.display();
    }

    template<typename T>
    void drawEdge(sf::RenderTexture& window, Vector2<T> origin, Vector2<T> destination, sf::Color color, unsigned width, unsigned height)
    {
        auto line = std::array<sf::Vertex, 2>
        {
            sf::Vertex(sf::Vector2f(origin.x * width, origin.y * height), color),
                sf::Vertex(sf::Vector2f(destination.x * width, destination.y * height), color)
        };
        window.draw(line.data(), 2, sf::Lines);
        window.display();
    }

    template<typename T>
    void drawPoints(sf::RenderTexture& window, const Diagram<T>& diagram, unsigned width, unsigned height)
    {
        for (const auto& site : diagram.getSites())
            drawPoint(window, site.point, sf::Color(100, 250, 50), width, height);
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
 
    static void drawPointsBuffer(sf::RenderTexture& rTex, const std::vector<delaunator::Point>& points, sf::Color color = sf::Color::Red)
    {
        for (auto pt : points)
        {
            auto shape = sf::CircleShape(PointRadius);
            shape.setPosition(sf::Vector2f(pt.x() - PointRadius, pt.y() - PointRadius));
            shape.setFillColor(color);
            rTex.draw(shape);
            rTex.display();
        }
    }

}
}