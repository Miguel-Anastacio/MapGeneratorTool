#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <chrono>
#include <MyGAL/Vector2.h>
#include <unordered_set>
#include "../utils/Color.h"
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
        }

        assert(colorsInUse.size() == polygons.size());
        texture.display();
    }

    static void saveToFile(const sf::RenderTexture& texture, const char* filename)
    {
        texture.getTexture().copyToImage().saveToFile(filename);
    }

    static void drawBuffer(const std::vector<sf::Uint8>& buffer, sf::RenderTexture& renderTexture, unsigned width, unsigned height)
    {
        auto start = std::chrono::steady_clock::now();
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
        auto duration = std::chrono::steady_clock::now() - start;
        std::cout << "draw buffer: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
    }
}
}