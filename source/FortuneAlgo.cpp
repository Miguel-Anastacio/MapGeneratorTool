#pragma once
#include "FortuneAlgo.h"
#include <unordered_set>
#include <SFML/Graphics.hpp>
namespace MapGeneratorTool
{
    /*namespace FortuneAlgo
    {
      */  //std::vector<Vector2> generatePoints(int nbPoints)
        //{
        //    uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
        //    std::cout << "seed: " << seed << '\n';
        //    std::default_random_engine generator(seed);
        //    std::uniform_real_distribution<double> distribution(0.0, 1.0);

        //    std::vector<Vector2> points;
        //    for (int i = 0; i < nbPoints; ++i)
        //        points.push_back(Vector2{ distribution(generator), distribution(generator) });

        //    return points;
        //}


        //VoronoiDiagram generateRandomDiagram(std::size_t nbPoints)
        //{
        //    // Generate points
        //    std::vector<Vector2> points = generatePoints(nbPoints);

        //    // Construct diagram
        //    FortuneAlgorithm algorithm(points);
        //    auto start = std::chrono::steady_clock::now();
        //    algorithm.construct();
        //    auto duration = std::chrono::steady_clock::now() - start;
        //    std::cout << "construction: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';

        //    // Bound the diagram
        //    start = std::chrono::steady_clock::now();
        //    algorithm.bound(Box{ -0.05, -0.05, 1.05, 1.05 }); // Take the bounding box slightly bigger than the intersection box
        //    duration = std::chrono::steady_clock::now() - start;
        //    std::cout << "bounding: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
        //    VoronoiDiagram diagram = algorithm.getDiagram();

        //    // Intersect the diagram with a box
        //    start = std::chrono::steady_clock::now();
        //    bool valid = diagram.intersect(Box{ 0.0, 0.0, 1.0, 1.0 });
        //    duration = std::chrono::steady_clock::now() - start;
        //    std::cout << "intersection: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
        //    if (!valid)
        //        throw std::runtime_error("An error occured in the box intersection algorithm");


        //    
        //    return diagram;
        //}

        //void drawEdge(sf::RenderTexture& window, Vector2 origin, Vector2 destination, sf::Color color)
        //{
        //    sf::Vertex line[] =
        //    {
        //        sf::Vertex(sf::Vector2f(origin.x, 1.0f - origin.y), color),
        //        sf::Vertex(sf::Vector2f(destination.x, 1.0f - destination.y), color)
        //    };
        //    window.draw(line, 2, sf::Lines);
        //}

        //void drawDiagram(VoronoiDiagram& diagram)
        //{
        //    sf::RenderTexture renderTexture;
        //    if (!renderTexture.create(800, 600)) {
        //        // Handle error
        //        //return -1;
        //    }
        //    renderTexture.clear(sf::Color::Blue);

        //    for (std::size_t i = 0; i < diagram.getNbSites(); ++i)
        //    {
        //        const VoronoiDiagram::Site* site = diagram.getSite(i);
        //        Vector2 center = site->point;
        //        VoronoiDiagram::Face* face = site->face;
        //        VoronoiDiagram::HalfEdge* halfEdge = face->outerComponent;
        //        if (halfEdge == nullptr)
        //            continue;
        //        while (halfEdge->prev != nullptr)
        //        {
        //            halfEdge = halfEdge->prev;
        //            if (halfEdge == face->outerComponent)
        //                break;
        //        }
        //        VoronoiDiagram::HalfEdge* start = halfEdge;
        //        while (halfEdge != nullptr)
        //        {
        //            if (halfEdge->origin != nullptr && halfEdge->destination != nullptr)
        //            {
        //                Vector2 origin = (halfEdge->origin->point - center) * OFFSET + center;
        //                Vector2 destination = (halfEdge->destination->point - center) * OFFSET + center;
        //                drawEdge(renderTexture, origin, destination, sf::Color::Red);
        //            }
        //            halfEdge = halfEdge->next;
        //            if (halfEdge == start)
        //                break;
        //        }
        //    }

        //    renderTexture.display();
        //    // Get the texture from the render texture
        //    const sf::Texture& texture = renderTexture.getTexture();

        //    // Get the image from the texture
        //    sf::Image image = texture.copyToImage();

        //    // Save the image to a file
        //    if (!image.saveToFile("output.png")) {
        //        // Handle error
        //        //return -1;
        //    }
        //}

        //std::unordered_map<Vector2, Color> GenerateColorMap(VoronoiDiagram& diagram)
        //{
        //    std::unordered_map<Vector2, Color> colorMap;
        //    std::unordered_set<Color> colorsInUse;

        //    for (std::size_t i = 0; i < diagram.getNbSites(); ++i)
        //    {
        //        Color color;
        //        do {
        //            color.RandColor();
        //        } while (colorsInUse.contains(color));

        //        colorMap.emplace(diagram.getSite(i)->point, color);

        //    }

        //    return colorMap;
        //}

        //void DrawDiagramToTexture(unsigned width, unsigned height, const std::unordered_map<Vector2, Color>& colorMap, VoronoiDiagram& diagram, char* filename)
        //{
        //    std::vector<uint8_t> image(width * height * 4);
        //    for (int y = 0; y < height; y++)
        //    {
        //        for (int x = 0; x < width; x++)
        //        {
        //            //Vector2 pt = diagram[width * y + x];
        //           /* image[4 * width * y + 4 * x + 0] = colorMap.at(pt).R;
        //            image[4 * width * y + 4 * x + 1] = colorMap.at(pt).G;
        //            image[4 * width * y + 4 * x + 2] = colorMap.at(pt).B;
        //            image[4 * width * y + 4 * x + 3] = 255;*/
        //        }
        //    }



        //}
    //}
}