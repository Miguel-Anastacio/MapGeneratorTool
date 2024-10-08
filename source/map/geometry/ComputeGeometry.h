#pragma once
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include "Color.h"
#include "MyGAL/Diagram.h"
#include "MyGAL/FortuneAlgorithm.h"
#include "Mask.h"
#include "delaunator/include/delaunator.hpp"
namespace MapGeneratorTool
{

	namespace geomt
	{
		// ------------------------ MYGAL 
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
		std::vector<mygal::Vector2<T>> generatePoints(int nbPoints, int seedValue )
		{
			std::cout << "seed: " << seedValue << '\n';
			auto generator = std::default_random_engine(seedValue);
			auto distribution = std::uniform_real_distribution<T>(0.0, 1.0);

			auto points = std::vector<mygal::Vector2<T>>(nbPoints);
			for (auto i = 0; i < nbPoints; ++i)
				points[i] = mygal::Vector2<T>(distribution(generator), distribution(generator));

			return points;
		}

		template<typename T>
		std::vector<mygal::Vector2<T>> generatePointsConstrained(int nbPoints, int seedValue, bool state, const Mask& mask)
		{
						
			std::cout << "seed: " << seedValue << '\n';
			std::default_random_engine generator(seedValue);
			std::uniform_real_distribution<T> distribution(0.0, 1.0);

			std::vector<mygal::Vector2<T>> points;
			points.reserve(nbPoints);

			while (points.size() < nbPoints) 
			{
				T x = distribution(generator);
				T y = distribution(generator);
				int maskX = static_cast<int>(x * mask.width);
				int maskY = static_cast<int>(y * mask.height);

				if (mask.isInMask(maskX, maskY)) 
				{
					points.emplace_back(x, y);
				}
			}

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
		// ------------------------ MYGAL
	}

	namespace rb
	{
		static std::vector<delaunator::Point> GeneratePoints(unsigned width, unsigned height, double jitter, int seeds = 100)
		{
			std::vector<delaunator::Point> points(width * height);
			auto seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::cout << "seed: " << seed << '\n';
			auto generator = std::default_random_engine(seed);
			auto distribution = std::uniform_real_distribution<double>(0.0, 1.0);

			for (size_t y = 0; y < height; y++)
			{
				for (size_t x = 0;  x < width;  x++)
				{
					points[y*width + x] = delaunator::Point(x + jitter * (distribution(generator) - distribution(generator)),
																y + jitter * (distribution(generator) - distribution(generator)));
				}
			}

			return points;
		}

		static std::vector<delaunator::Point> calculateCentroids(const std::vector<delaunator::Point>& points, delaunator::Delaunator& delaunay)
		{
			const int numTriangles = delaunay.halfedges.size() / 3;
			std::vector<delaunator::Point> centroids;
			for (size_t t = 0; t < numTriangles; t++) 
			{
				double sumOfX = 0, sumOfY = 0;
				for (int i = 0; i < 3; i++) 
				{
					double s = 3 * t + i;
					delaunator::Point p = points[delaunay.triangles[s]];
					sumOfX += p.x();
					sumOfY += p.y();
				}
				centroids[t] = delaunator::Point(sumOfX / 3.0, sumOfY / 3.0);
			}
			return centroids;
		}

		static void Delaunay(const std::vector<delaunator::Point>& points)
		{
			std::vector<double> coords;
			for (auto pt : points)
			{
				coords.push_back(pt.x());
				coords.push_back(pt.y());
			}
			delaunator::Delaunator del(coords);
		}

	}

	
}