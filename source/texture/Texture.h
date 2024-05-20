#pragma once
#include <vector>
namespace MapGeneratorTool
{
	class Texture
	{
	public:
		Texture(unsigned width, unsigned height, const char* name) :
			m_width(width), m_height(height), m_name(name), m_buffer(width* height * 4) {};
		Texture(const char* filename);
		Texture(const Texture& other) : m_width(other.m_width), m_height(other.m_height), m_name(other.m_name), m_buffer(other.m_buffer) {};
		~Texture() {};

		void ReadTextureFromFile(const char* filename);
		void WriteTextureToFile(const char* filename) const;
		void WriteTextureToFile() const;

		inline void SetBuffer(const std::vector<uint8_t>& buffer) {
			m_buffer = buffer;
		}

		inline unsigned width()  const {
			return m_width;
		}
		inline unsigned height()  const{
			return m_height;
		}

		//Texture& operator =(const Texture& other);
	private:
		unsigned m_width;
		unsigned m_height;
		const char* m_name;
		std::vector<uint8_t> m_buffer;
	};

}
