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

		struct Color
		{
			uint8_t R;
			uint8_t G;
			uint8_t B;
			uint8_t A;

			Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : R(r), G(g), B(b), A(a) {};
			Color() : R(255), G(255), B(255), A(0) {};
			Color(const Color& color)
			{
				*this = color;
			}
			Color& operator=(const Color& other)
			{
				if (this == &other) return *this;

				this->R = other.R;
				this->G = other.G;
				this->B = other.B;
				this->A = other.A;
			}
			bool operator==(const Color& other) const
			{
				return R == other.R && G == other.G && B == other.B && A == other.A;
			}

			void RandColor()
			{
				R = static_cast<uint8_t>(rand() % 255);
				G = static_cast<uint8_t>(rand() % 255);
				B = static_cast<uint8_t>(rand() % 255);
				A = 255;
			}
		};

		static float distanceSquared(const Point& a, const Point& b)
		{
			return pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2);
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

	template <>
	struct hash<MapGeneratorTool::Utils::Color> {
		std::size_t operator()(const MapGeneratorTool::Utils::Color& color) const
		{
			return std::hash<int>()(color.R) ^ (std::hash<int>()(color.G) << 1) ^ (std::hash<int>()(color.B) << 1) ^ (std::hash<int>()(color.A) << 1);
		}
	};
}