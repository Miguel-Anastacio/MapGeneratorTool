#include <SFML/Graphics.hpp>
#include <cassert>
#include <MyGAL/Vector2.h>
namespace MapGeneratorTool
{
   /* template<typename T>
    void drawPolygon(const std::vector<Vector2<T>>& polygon, std::unordered_set<Utils::Color>& colorsInUse;)
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
    }*/
    using namespace  mygal;
    template<typename T>
    void drawPolygons(const std::vector<std::vector<Vector2<T>>>& polygons, sf::RenderTexture& texture, unsigned width, unsigned height)
    {
        if (!texture.create(width, height))
        {
            // error...
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
}