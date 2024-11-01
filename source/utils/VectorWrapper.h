#include "MyGAL/Vector2.h"
namespace mygal
{
	template<typename T>
	static mygal::Vector2<T> operator/(const mygal::Vector2<T>& vector, T div)
	{
		return mygal::Vector2<T>(vector.x / div, vector.y / div);
	}

	template<typename T>
    static mygal::Vector2<T> operator*(const mygal::Vector2<T>& vector, T scale)
	{
		return mygal::Vector2<T>(vector.x * scale, vector.y * scale);
	}

    template<typename T>
    static mygal::Vector2<T> operator*(const mygal::Vector2<T>& vector, const mygal::Vector2<K>& scale)
	{
		return mygal::Vector2<T>(vector.x * scale.x, vector.y * scale.y);
	}

    template<typename T>
    static mygal::Vector2<T> operator/(const mygal::Vector2<T>& vector, const mygal::Vector2<K>& scale)
	{
		return mygal::Vector2<T>(vector.x / scale.x, vector.y / scale.y);
	}

    template<typename T>
    static void operator*=(mygal::Vector2<T>& vector, T scale)
	{
		vector.x *= scale;
		vector.y *= scale;
	}

    template<typename T>
    static void operator/=(mygal::Vector2<T>& vector, T scale)
	{
		vector.x /= scale;
		vector.y /= scale;
	}
}

// namespace MapGeneraotTool
// {    
// namespace Utils
// {
// namespace vector
// {

// using mygal;
// template<typename T, typename K>
// static mygal::Vector2<K> ScaleByVector(const mygal::Vector2<T>& og, const mygal::Vector2<K>& scale)
// {
//     return mygal::Vector2<K>(og.x * scale.x, og.y * scale.y);
// }

// template<typename T, typename K>
// static mygal::Vector2<T> ScaleBothAxis(const mygal::Vector2<T>& og, K xScale, K yScale)
// {
//     return mygal::Vector2<T>(og.x * xScale, og.y * yScale);
// }

// template<typename T, typename K>
// static mygal::Vector2<K> DivideByVector(const mygal::Vector2<T>& og, const mygal::Vector2<K>& scale)
// {
//     return mygal::Vector2<K>(og.x / scale.x, og.y / scale.y);
// }

// } // namespace Utils
// } // namespace Vector
// } // namespace MapGeneraotTool


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
