#pragma once
namespace MapGeneratorTool
{
	struct LookupMapData
	{
		unsigned width = 10;
		unsigned height = 10;
		int seed = 0;
		int numberOfSeeds = 5;
		int lloyd = 0;

		LookupMapData(unsigned w, unsigned h, int s, int sNum, int l = 0) : width(w), height(h), seed(s), numberOfSeeds(sNum), lloyd(l)
		{};
		LookupMapData() = default;
	};
}