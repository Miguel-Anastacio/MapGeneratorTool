#pragma once
#include "components/HeightMap.h"
#include "Map.h"
#include "Renderer.h"
#include "lodepng/textureHandler.h"
namespace MapGeneratorTool
{
enum class SaveType
{
	Lookup, Terrain, Noise
};

class Event
{
public:
	 Event() {};
	~Event() = default;

	virtual void Execute(Map& map) const = 0;

};

class GenerateLookupMapEvent : public Event
{
public:
	GenerateLookupMapEvent(const LookupMapData& data) : m_data(data){}

	void Execute(Map& map) const override 
	{
		map.RegenerateLookUp(m_data);
	}
private:
	LookupMapData m_data;
};

class GeneratNoiseMapEvent : public Event
{
public:
	GeneratNoiseMapEvent(NoiseMapData data) : m_data(data) {}

	void Execute(Map& map) const override
	{
		map.GenerateHeightMapTectonic(m_data);
	}
private:
	NoiseMapData m_data;
};

class GenerateTerrainMapEvent : public Event
{
public:
	GenerateTerrainMapEvent(TerrainType& type) : m_terrainType(std::move(type)), flag(true) {};
	GenerateTerrainMapEvent() : m_terrainType(TerrainType()) {};
	GenerateTerrainMapEvent(const std::vector<TerrainType>&typeVector) : m_terrainType(TerrainType()), m_terrainTypeVector(typeVector) {};

	void Execute(Map& map) const override
	{
		if (flag)
		{
			map.AddTerrainType(m_terrainType);
		}
		map.GenerateTerrainMap(map.NoiseMap(), m_terrainTypeVector);
	}
private:
	const TerrainType m_terrainType;
	const std::vector<TerrainType> m_terrainTypeVector;
	bool flag = false;

};

class SaveEvent : public Event
{
public:
	SaveEvent(const std::string& path) : filepath(path){};
	void Execute(Map& map) const override
	{
		map.SaveMap(filepath);
	}
private:
	std::string filepath;
};


class LoadHeightMapEvent : public Event
{
public:
	LoadHeightMapEvent(const std::string& name) : filename(name) {};
	void Execute(Map& map) const override
	{
		map.Reset();
		unsigned width, height = 0;
		auto heightMapBuffer = textureHandler::decodeOneStep(filename.c_str(), width, height);
		map.GenerateMap(heightMapBuffer, width, height);

	}
private:
	std::string filename;
};

class BorderNoiseEvent : public Event
{
public:
	BorderNoiseEvent(const LookupMapData& data) : m_data(data) {}
	void Execute(Map& map) const override
	{
		map.RegenerateLookupBorders(m_data);
	}
private:
	LookupMapData m_data;
};

}