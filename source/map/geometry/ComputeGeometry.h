#pragma once
#include <vector>
#include <random>
#include "MyGAL/Diagram.h"
#include "MyGAL/FortuneAlgorithm.h"
#include "Mask.h"
#include <cmath>
#include "fastNoiseLite/FastNoiseLite.h"
#include "VectorWrapper.h"
#include "Logger.h"
#include "Timer.h"
namespace MapGeneratorTool
{
	namespace geomt
	{
		const float PI = 3.14159265359f;

		// ------------------------ MYGAL 
		template<typename T>
		static std::vector<mygal::Vector2<T>> generatePoints(int nbPoints)
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
		static std::vector<mygal::Vector2<T>> generatePoints(int nbPoints, int seedValue )
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
		static std::vector<mygal::Vector2<T>> generatePointsConstrained(int nbPoints, int seedValue, bool state, const Mask& mask)
		{
			//Timer 			
			std::cout << "seed: " << seedValue << '\n';
			std::default_random_engine generator(seedValue);
			std::uniform_real_distribution<T> distribution(0.0, 1.0);

			std::vector<mygal::Vector2<T>> points;
			points.reserve(nbPoints);

			while (points.size() < nbPoints) 
			{
				T x = distribution(generator);
				T y = distribution(generator);
				int maskX = static_cast<int>(x * mask.Width());
				int maskY = static_cast<int>(y * mask.Height());

				if (mask.isInMask(maskX, maskY)) 
				{
					points.emplace_back(x, y);
				}
			}

			return points;
		}

		template<typename T>
		static mygal::Diagram<T> generateDiagram(const std::vector<mygal::Vector2<T>>& points)
		{
			// Construct diagram
			auto algorithm = mygal::FortuneAlgorithm<T>(points);
			algorithm.construct();

			// Bound the diagram
			algorithm.bound(mygal::Box<T>{-0.05, -0.05, 1.05, 1.05}); // Take the bounding box slightly bigger than the intersection box
			auto diagram = algorithm.getDiagram();

			// Intersect the diagram with a box
			diagram.intersect(mygal::Box<T>{-0.01, -0.01, 1.01, 1.01});

			return std::move(diagram);
		}

		template<typename T>
		static void lloydRelaxation(mygal::Diagram<T>& diagram, int iterations, const Mask& mask)
		{
			for (int i = 0; i < iterations; i++)
			{
				std::vector<mygal::Vector2<double>> centroids = diagram.computeLloydRelaxation();
				/*for (auto& point : centroids)
				{
					 auto newPoint = mask.getClosestPointInMask(point);
					 point = newPoint;
				}*/
				diagram = std::move(geomt::generateDiagram(centroids));
			}
		}

		template<typename T>
		static void lloydRelaxation(mygal::Diagram<T>& diagram, int iterations)
		{
			for (int i = 0; i < iterations; i++)
			{
				std::vector<mygal::Vector2<double>> centroids = diagram.computeLloydRelaxation();
				diagram = std::move(geomt::generateDiagram(centroids));
			}
		}

		template<typename T>
		static std::vector<mygal::Vector2<T>> GetCentroidsOfDiagram(mygal::Diagram<T>& diagram)
		{
			std::vector<mygal::Vector2<T>> centroids;
			centroids.reserve(diagram.getSites().size());
			for (auto& site : diagram.getSites())
			{
				centroids.emplace_back(site.point);
			}
			return centroids;
		}


		// ------------------------ MYGAL



		template<typename T>
		static std::vector<T> sampleNoiseInACircle(const FastNoiseLite& noise, float radius, int numSamples, float offsetX = 0.0f, float offsetY = 0.0f)
		{
			std::vector<T> circleNoise(numSamples);
			for (int i = 0; i < numSamples; ++i) {
				// Sample at different angles around the circle
				T theta = (2 * PI * i) / numSamples;
				T x = radius * cos(theta) + offsetX;
				T y = radius * sin(theta) + offsetY;

				// Sample 2D noise
				circleNoise[i] = noise.GetNoise(x, y);
			}
			return circleNoise;
		}

		// Function to normalize the noise values
		template<typename T>
		static void normalizeNoise(std::vector<T>& noise, int cutPointIndex) {
			T cutPointValue = noise[cutPointIndex];
			for (auto& val : noise) 
			{
				val -= cutPointValue; // Normalize to make noise at cut point zero
			}
		}

		// Function to map circle noise to a 1D fault line
		template<typename T>
		static void applyNoiseToLine(const std::vector<T>& circleNoise, std::vector<mygal::Vector2<T>>& line, int cutPointIndex, float noiseScale, int thickness)
		{
			auto numSamples = circleNoise.size();
			std::vector<T> faultLine(numSamples);

			// Map circle noise to fault line, with cut point at both ends

			faultLine[0] = circleNoise[cutPointIndex];
			faultLine[numSamples-1] = circleNoise[cutPointIndex];
			for (int i = 1; i < numSamples-1; ++i) 
			{
				faultLine[i] = circleNoise[i];
			}

			int index = 0;
			for (int i = 0; i < numSamples; ++i)
			{
				mygal::Vector2<T> tangent = line.size() > 1 ? line[numSamples-1] - line[0] : mygal::Vector2<T>(1, 0); // Approximate tangent
				auto perpendicular = mygal::normed(tangent.getOrthogonal());

				line[i].x += faultLine[i] * noiseScale * perpendicular.x;
				line[i].y += faultLine[i] * noiseScale * perpendicular.y;
			}

		}

		template<typename T>
		static void DeformLine(std::vector<mygal::Vector2<T>>& line, FastNoiseLite& noise, float noiseScale = 25.0f, int thickness = 2)
		{
			// Initialize the noise generator
			//int seed = std::rand();
			noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

			// Parameters
			float radius = 1.0f;      // Circle radius
			size_t numSamples = line.size();     // Number of samples (points on the circle)
			const int maxvalue = INT32_MAX;
			int cutPointIndex = 0;    // Cut point (can be any index on the circle)

			// Step 1: Sample noise on the circle
			std::vector<double> circleNoise = sampleNoiseInACircle<double>(noise, radius, (int)numSamples);

			// Step 2: Normalize noise so that the value at the cut point is zero
			normalizeNoise(circleNoise, cutPointIndex);

			// Step 3: Map circle noise to a fault line
			applyNoiseToLine(circleNoise, line, cutPointIndex, noiseScale, thickness);

		}
	}

	

	//namespace rb
	//{
	//	static std::vector<delaunator::Point> GeneratePoints(unsigned width, unsigned height, double jitter, int seeds = 100)
	//	{
	//		std::vector<delaunator::Point> points(width * height);
	//		auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	//		std::cout << "seed: " << seed << '\n';
	//		auto generator = std::default_random_engine(seed);
	//		auto distribution = std::uniform_real_distribution<double>(0.0, 1.0);

	//		for (size_t y = 0; y < height; y++)
	//		{
	//			for (size_t x = 0;  x < width;  x++)
	//			{
	//				points[y*width + x] = delaunator::Point(x + jitter * (distribution(generator) - distribution(generator)),
	//															y + jitter * (distribution(generator) - distribution(generator)));
	//			}
	//		}

	//		return points;
	//	}

	//	/*static std::vector<delaunator::Point>	calculateCentroids(const std::vector<delaunator::Point>& points, delaunator::Delaunator& delaunay)
	//	{
	//		const int numTriangles = (int)delaunay.halfedges.size() / 3;
	//		std::vector<delaunator::Point> centroids;
	//		for (size_t t = 0; t < numTriangles; t++) 
	//		{
	//			double sumOfX = 0, sumOfY = 0;
	//			for (int i = 0; i < 3; i++) 
	//			{
	//				double s = 3 * t + i;
	//				delaunator::Point p = points[delaunay.triangles[s]];
	//				sumOfX += p.x();
	//				sumOfY += p.y();
	//			}
	//			centroids[t] = delaunator::Point(sumOfX / 3.0, sumOfY / 3.0);
	//		}
	//		return centroids;
	//	}

	//	static void Delaunay(const std::vector<delaunator::Point>& points)
	//	{
	//		std::vector<double> coords;
	//		for (auto pt : points)
	//		{
	//			coords.push_back(pt.x());
	//			coords.push_back(pt.y());
	//		}
	//		delaunator::Delaunator del(coords);
	//	}*/

	//}

	
}