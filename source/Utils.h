#pragma once
#include <unordered_map>
#include <vector>
namespace MapGeneratorTool
{
	namespace Utils
	{
		

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
	}

}

