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

	struct NoiseMapData
	{

		unsigned width = 10;
		unsigned height = 10;
		int seed = 100123;
		int octaves = 5;
		float frequency = 0.01f;
		float scale = 1.f;
		float lacunarity = 2.0f;
		NoiseMapData(unsigned w, unsigned h, int s = 100123, int oct = 5, float f = 0.01, float scl = 1.0f, float lac = 2.0) :
			width(w), height(h), seed(s), octaves(oct), frequency(f), scale(scl), lacunarity(lac)
		{};
		NoiseMapData() = default;
	};
}