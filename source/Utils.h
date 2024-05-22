#pragma once
#include <unordered_map>
#include <vector>
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
			Color() : R(255), G(255), B(255), A(255) {};
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
				return *this;
			}
			bool operator==(const Color& other) const
			{
				return R == other.R && G == other.G && B == other.B && A == other.A;
			}

		/*	Color& operator*=(float scale)
			{

				this->R *= scale;
				this->G *= scale;
				this->B *= scale;
				return *this;
			}*/

			void RandColor()
			{
				R = static_cast<uint8_t>(rand() % 255);
				G = static_cast<uint8_t>(rand() % 255);
				B = static_cast<uint8_t>(rand() % 255);
				A = 255;
			}
		};

		class Dimensions
		{
		public:
			Dimensions(unsigned width, unsigned height) : m_width(width), m_height(height) {};
			Dimensions() : m_width(1), m_height(1) {};

			inline unsigned width()  const {
				return m_width;
			}
			inline unsigned height()  const {
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
		std::vector<T> mapValuesToVector(std::unordered_map<K, T> map)
		{
			std::vector<T> values;
			values.reserve(map.size());
			for (const auto val : map)
			{
				values.push_back(val.second);
			}

			return values;
		}

		static float distanceSquared(const Point& a, const Point& b)
		{
			return pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2);
		}


		static float inverseLerp(float xx, float yy, float value)
		{
			return (value - xx) / (yy - xx);
		}

		static const Color operator *(const Color& color, float scale)
		{
			return Color(color.R * scale, color.G * scale, color.B * scale, color.A);
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