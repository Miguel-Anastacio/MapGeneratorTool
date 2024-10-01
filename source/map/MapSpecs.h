#pragma once
namespace MapGeneratorTool
{
	struct LookupFeatures
	{
		int seed = 0;
		int tiles = 5;
		int lloyd = 0;
		LookupFeatures(int s, int pts, int l) : seed(s), tiles(pts), lloyd(l) {};
		LookupFeatures() = default;
	};

	struct LookupMapData
	{
		unsigned width = 10;
		unsigned height = 10;
		LookupFeatures land;
		LookupFeatures ocean;
		float cutOffHeight = 0.001f;

		LookupMapData(unsigned w, unsigned h, LookupFeatures landSpec, LookupFeatures oceanSpec, float cutOff) : 
			width(w), height(h), 
			land(landSpec),
			ocean(oceanSpec),
			cutOffHeight(cutOff)
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