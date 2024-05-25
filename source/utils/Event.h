#pragma once
#include "../map/MapSpecs.h"
#include "../map/Map.h"
namespace MapGeneratorTool
{
//enum class EventType
//{
//	GenerateMap, GenerateTerrain
//};

class Map;
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


class SaveMapEvent : public Event
{
public:
	SaveMapEvent(const char* name) : filename(name) {};
	void Execute(Map& map) const override
	{
		map.SaveDiagramToFile(filename);
	}
private:
	const char* filename;
};

}