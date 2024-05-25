#pragma once
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include "utils/Color.h"
#include "MyGAL/Diagram.h"
#include "MyGAL/FortuneAlgorithm.h"
namespace MapGeneratorTool
{
	//namespace SimpleVoronoiDiagram
	//{
	//using namespace Utils;
	//using ::std::vector;
	//using ::std::unordered_map;

	//vector<Utils::Point> GenerateSeeds(int numberOfSeeds, int width, int height);
	////unordered_map<Utils::Point, Utils::Color> GenerateColorMap(const vector<Point>& seeds);

	//vector<Point> GenerateDiagram(const vector<Point>& seeds, int width, int height);

	//vector<Point> GenerateDiagramFromMask(const vector<Point>& seeds, int width, int height, const vector<uint8_t> mask);

	//vector<Point> ComputeCentroids(const vector<Point>& diagram, int width, int height, const vector<Point>& seeds);

	//template <typename T>
	//void WriteDiagramToTexture(const Texture& texture, mygal::Diagram<T>& diagram);

	//template<typename T>
	//void WriteDiagramToTexture(const Texture& texture, mygal::Diagram<T>& diagram)
	//{

	//}

	//}

	namespace geomt
	{

		template<typename T>
		std::vector<mygal::Vector2<T>> generatePoints(int nbPoints)
		{
			auto seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::cout << "seed: " << seed << '\n';
			auto generator = std::default_random_engine(seed);
			auto distribution = std::uniform_real_distribution<T>(0.0, 1.0);

			auto points = std::vector<mygal::Vector2<T>>(nbPoints);
			for (auto i = 0; i < nbPoints; ++i)
				points[i] = mygal::Vector2<T>(distribution(generator), distribution(generator));

			return points;
		}

		template<typename T>
		mygal::Diagram<T> generateDiagram(const std::vector<mygal::Vector2<T>>& points)
		{
			// Construct diagram
			auto algorithm = mygal::FortuneAlgorithm<T>(points);
			auto start = std::chrono::steady_clock::now();
			algorithm.construct();
			auto duration = std::chrono::steady_clock::now() - start;
			std::cout << "construction: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';

			// Bound the diagram
			start = std::chrono::steady_clock::now();
			algorithm.bound(mygal::Box<T>{-0.05, -0.05, 1.05, 1.05}); // Take the bounding box slightly bigger than the intersection box
			duration = std::chrono::steady_clock::now() - start;
			std::cout << "bounding: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
			auto diagram = algorithm.getDiagram();

			// Intersect the diagram with a box
			start = std::chrono::steady_clock::now();
			diagram.intersect(mygal::Box<T>{0.0, 0.0, 1.0, 1.0});
			duration = std::chrono::steady_clock::now() - start;
			std::cout << "intersection: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';

			return diagram;
		}

		template<typename T>
		void lloydRelaxation(mygal::Diagram<T>& diagram, int iterations)
		{
			for (int i = 0; i < iterations; i++)
			{
				std::vector<mygal::Vector2<double>> centroids = diagram.computeLloydRelaxation();
				diagram = std::move(geomt::generateDiagram(centroids));
			}
		}
	}

	
}