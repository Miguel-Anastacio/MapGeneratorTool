#pragma once
#include "../map/MapSpecs.h"
#include "../map/Map.h"
namespace MapGeneratorTool
{
//enum class EventType
//{
//	GenerateMap, GenerateTerrain
//};

class Event
{
public:
	Event() {};
	~Event() = default;

	virtual void Execute(Map& map) const = 0;
private:

};

class GenerateLookupMapEvent : public Event
{
public:
	GenerateLookupMapEvent(LookupMapData data) : m_data(data){}

	void Execute(Map& map) const override 
	{
		map.GenerateMap(m_data);

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
		map.GenerateHeightMap(m_data);
	}
private:
	NoiseMapData m_data;
};


class SaveLookupMapEvent : public Event
{
public:
	SaveLookupMapEvent(const char* name) : filename(name) {};
	void Execute(Map& map) const override
	{
		map.SaveLookupMapToFile(filename);
	}
private:
	const char* filename;
};

class SaveNoiseMapEvent : public Event
{
public:
	SaveNoiseMapEvent(const char* name) : filename(name) {};
	void Execute(Map& map) const override
	{
		map.SaveHeightMapToFile(filename);
	}
private:
	const char* filename;
};

}