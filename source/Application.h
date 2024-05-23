#pragma once
#include <cmath>
#include <vector>
#include <ctime>
#include <chrono>
#include <random>
#include <iostream>
#include <cassert>
#include "MyGAL/FortuneAlgorithm.h"
#include "MyGAL/Vector2.h"
#include <SFML/Graphics.hpp>
#include "utils/Color.h"
namespace MapGeneratorTool
{
    using namespace  mygal;
    template<typename T>
    void drawPolygons(const std::vector<std::vector<Vector2<T>>>& polygons)
    {
        int width = 1024, height = 1024;
        sf::RenderTexture renderTexture;
        if (!renderTexture.create(width, height))
        {
            // error...
        }
        std::unordered_set<Utils::Color> colorsInUse;
        // drawing uses the same functions
        renderTexture.clear();
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
            renderTexture.draw(convex);
        }

        assert(colorsInUse.size() == polygons.size());

        renderTexture.display();
        renderTexture.getTexture().copyToImage().saveToFile("toto.png");
    }


    template<typename T>
    std::vector<Vector2<T>> generatePoints(int nbPoints)
    {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::cout << "seed: " << seed << '\n';
        auto generator = std::default_random_engine(seed);
        auto distribution = std::uniform_real_distribution<T>(0.0, 1.0);

        auto points = std::vector<Vector2<T>>(nbPoints);
        for (auto i = 0; i < nbPoints; ++i)
            points[i] = Vector2<T>(distribution(generator), distribution(generator));

        return points;
    }


    template<typename T>
    Diagram<T> generateDiagram(const std::vector<Vector2<T>>& points)
    {
        // Construct diagram
        auto algorithm = FortuneAlgorithm<T>(points);
        auto start = std::chrono::steady_clock::now();
        algorithm.construct();
        auto duration = std::chrono::steady_clock::now() - start;
        std::cout << "construction: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';

        // Bound the diagram
        start = std::chrono::steady_clock::now();
        algorithm.bound(Box<T>{-0.05, -0.05, 1.05, 1.05}); // Take the bounding box slightly bigger than the intersection box
        duration = std::chrono::steady_clock::now() - start;
        std::cout << "bounding: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
        auto diagram = algorithm.getDiagram();

        // Intersect the diagram with a box
        start = std::chrono::steady_clock::now();
        diagram.intersect(Box<T>{0.0, 0.0, 1.0, 1.0});
        duration = std::chrono::steady_clock::now() - start;
        std::cout << "intersection: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';

        return diagram;
    }
	void Run();
}