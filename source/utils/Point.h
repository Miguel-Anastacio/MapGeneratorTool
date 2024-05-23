#pragma once
namespace MapGeneratorTool
{
	namespace Utils
	{
		struct  Point
		{
			int X;
			int Y;
			Point(int xPos, int yPos) : X(xPos), Y(yPos) {};
			Point() : X(0), Y(0) {};
			Point(const Point& point)
			{
				*this = point;
			}
			Point& operator=(const Point& other)
			{
				if (this == &other) return *this;

				this->X = other.X;
				this->Y = other.Y;

				return *this;
			}

			bool operator ==(const Point& other) const
			{
				return this->X == other.X && this->Y == other.Y;
			}

			bool operator !=(const Point& other) const
			{
				return this->X != other.X || this->Y != other.Y;
			}

		};

		static float distanceSquared(const Point& a, const Point& b)
		{
			return pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2);
		}

		static float inverseLerp(float xx, float yy, float value)
		{
			return (value - xx) / (yy - xx);
		}
	}
}

namespace std {
	template <>
	struct hash<MapGeneratorTool::Utils::Point> {
		std::size_t operator()(const MapGeneratorTool::Utils::Point& point) const
		{
			return std::hash<int>()(point.X) ^ (std::hash<int>()(point.Y) << 1);
		}
	};
	//template <>
	//struct hash<MapGeneratorTool::Utils::Color> {
	//	std::size_t operator()(const MapGeneratorTool::Utils::Color& color) const
	//	{
	//		return std::hash<int>()(color.R) ^ (std::hash<int>()(color.G) << 1) ^ (std::hash<int>()(color.B) << 1) ^ (std::hash<int>()(color.A) << 1);
	//	}
	//};
}