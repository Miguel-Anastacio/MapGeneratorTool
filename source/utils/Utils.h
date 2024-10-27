#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "MyGAL/Vector2.h"
namespace MapGeneratorTool
{
	namespace Utils
	{
		class Dimensions
		{
		public:
			Dimensions(unsigned width, unsigned height) : m_width(width), m_height(height) {};
			Dimensions() : m_width(1), m_height(1) {};

			inline unsigned Width()  const {
				return m_width;
			}
			inline unsigned Height()  const {
				return m_height;
			}

		protected:
			inline void setDimensions(unsigned width, unsigned height) {
				m_width = width;
				m_height = height;
			}		

		private:
			unsigned m_width;
			unsigned m_height;

		};

		template<typename K, typename T> 
		std::vector<T> mapValuesToVector(const std::unordered_map<K, T>& map)
		{
			std::vector<T> values;
			values.reserve(map.size());
			for (const auto val : map)
			{
				values.push_back(val.second);
			}

			return values;
		}

		template<typename K, typename T>
		std::vector<T> mapKeysToVector(const std::unordered_map<K, T>& map)
		{
			std::vector<T> keys;
			keys.reserve(map.size());
			for (const auto val : map)
			{
				keys.push_back(val.first);
			}

			return keys;
		}

		static std::string WStringToString(const std::wstring& wstr)
		{
			std::string str;
			size_t size;
			str.resize(wstr.length());
			wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
			return str;
		}

		/*template<typename T>
		static void ScaleByVector(mygal::Vector2<T>& og, const mygal::Vector2<T>& scale)
		{
			og.x *= scale.x;
			og.x *= scale.y;
		}*/

		template<typename T, typename K>
		static mygal::Vector2<K> ScaleByVector(const mygal::Vector2<T>& og, const mygal::Vector2<K>& scale)
		{
			return mygal::Vector2<K>(og.x * scale.x, og.y * scale.y);
		}

		template<typename T, typename K>
		static mygal::Vector2<T> ScaleBothAxis(const mygal::Vector2<T>& og, K xScale, K yScale)
		{
			return mygal::Vector2<T>(og.x * xScale, og.y * yScale);
		}

		template<typename T, typename K>
		static mygal::Vector2<K> DivideByVector(const mygal::Vector2<T>& og, const mygal::Vector2<K>& scale)
		{
			return mygal::Vector2<K>(og.x * scale.x, og.y * scale.y);
		}

		//using namespace  mygal;
		// Function to calculate cross product of vectors (P1P2) and (P1P)
		//template<typename T>
		//float crossProduct(const Vector2<T>& P1, const Vector2<T>& P2, const Vector2<int>& P, unsigned width, unsigned height)
		//{
		//	Vector2<T> p1 = P1;
		//	Vector2<T> p2 = P2;
		//	//const Vector2<T> p = P;
		//	p1.x *= width;
		//	p2.x *= width;
		//	//p3.x*=width;

		//	p1.y *= height;
		//	p2.y *= height;
		//	//p3.y *= height;

		//	return (p2.x - p1.x) * (P.y - p1.y) - (p2.y - p1.y) * (P.x - p1.x);
		//}

		//template<typename T>
		//bool isPointInsidePolygon(const std::vector <Vector2<T>>& polygon, Vector2<int> point, unsigned width, unsigned height)
		//{
		//	int n = polygon.size();
		//	if (n < 3) return false;  // A polygon must have at least 3 points

		//	bool allPositive = false;
		//	bool allNegative = false;

		//	for (int i = 0; i < n; ++i)
		//	{
		//		// Get the current vertex and the next vertex (loop back at the end)
		//		const auto& P1 = polygon[i];
		//		const auto& P2 = polygon[(i + 1) % n];

		//		// Calculate cross product
		//		float cross = crossProduct(P1, P2, point, width, height);
		//		if (i == 0)
		//		{
		//			// Initialize the sign of the first cross product
		//			if (cross > 0) allPositive = true;
		//			else if (cross < 0) allNegative = true;
		//		}
		//		else {
		//			// Ensure that all cross products have the same sign
		//			if (allPositive && cross < 0) return false;
		//			if (allNegative && cross > 0) return false;
		//		}
		//	}

		//	// If all cross products are positive or all are negative, the point is inside
		//	return true;
		//}

	}

}


namespace std {
	template <>
	struct hash<mygal::Vector2<int>>
	{
		std::size_t operator()(const mygal::Vector2<int>& point) const
		{
			return std::hash<int>()(point.x) ^ (std::hash<int>()(point.y));
		}
	};
}