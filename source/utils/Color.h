#pragma once
#include <iostream>
#include <format>
namespace MapGeneratorTool
{
	namespace Utils
	{
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

			std::string ConvertToHex() const 
			{
				/*std::stringstream stream;
				stream << "#";
				stream << std::hex << std::setw(2) << std::setfill('0') << R;
				stream << std::hex << std::setw(2) << std::setfill('0') << G;
				stream << std::hex << std::setw(2) << std::setfill('0') << B;
				stream << std::hex << std::setw(2) << std::setfill('0') << A;
				return stream.str();*/

				return std::format("#{:02x}{:02x}{:02x}{:02x}", R, G, B, A);
			}

		};

		static const Color operator *(const Color& color, int scale)
		{
			return Color(color.R * scale, color.G * scale, color.B * scale, color.A);
		}
	}
}

namespace std {
	template <>
	struct hash<MapGeneratorTool::Utils::Color> {
		std::size_t operator()(const MapGeneratorTool::Utils::Color& color) const
		{
			return std::hash<int>()(color.R) ^ (std::hash<int>()(color.G) << 1) ^ (std::hash<int>()(color.B) << 1) ^ (std::hash<int>()(color.A) << 1);
		}
	};
}
