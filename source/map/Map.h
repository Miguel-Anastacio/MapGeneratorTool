#include <memory>
#include "../Utils.h"
#include"../utils/Point.h"
#include"../utils/Color.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <MyGAL/Diagram.h>
namespace MapGeneratorTool
{
	class Texture;
	using namespace Utils;
	class Map : public Dimensions
	{
	public:
		//Map(const char* maskFileName, int seeds, const char* lookUpTextureName,int iterLloyd);
		Map(unsigned width, unsigned height, int seeds, const char* lookUpTextureName, int iterLloyd=0);
		~Map();

	private:
		inline int valSeeds(int seeds)
		{
			return std::clamp(seeds, 0, static_cast<int>(std::pow(256, 3)));
		}
		void CreateLookUpTexture();
		void CreateLookUpTextureFromMask(const Texture& mask);

		void PopulateTexture(const std::unordered_map<Point, Color>& colorMap, const std::vector<Point>& diagram, Texture* texture) const;
		std::vector<uint8_t> GenerateMaskData(const Texture& mask) const;
		void OutputSeedPoints(const std::vector<Point>& seeds) const;

		void SaveDiagramToFile();
		

		int m_divisions;
		const char* m_lookupTextureName;
		mygal::Diagram<double> m_diagram;

		//std::unique_ptr<Texture> m_lookUpTexture;
	};

}