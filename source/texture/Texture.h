#pragma once
#include <vector>
#include "Utils.h"
namespace MapGeneratorTool
{
	using namespace Utils;
	class Texture : public Dimensions
	{
	public:
		Texture(unsigned width, unsigned height, const char* name) :
			Dimensions(width, height), m_name(name), m_buffer(width* height * 4) {};
		Texture(const char* filename);
		Texture(const Texture& other) : Dimensions(other.Width(), other.Height()), m_name(other.m_name), m_buffer(other.m_buffer) {};
		~Texture() {};

		void ReadTextureFromFile(const char* filename);
		void WriteTextureToFile(const char* filename) const;
		void WriteTextureToFile() const;

		inline void SetBuffer(const std::vector<uint8_t>& buffer) {
			m_buffer = buffer;
		}
		inline std::vector<uint8_t> GetBuffer() const{
			return m_buffer;
		}

		//Texture& operator =(const Texture& other);
	private:
;
		const char* m_name;
		std::vector<uint8_t> m_buffer;
	};

}
