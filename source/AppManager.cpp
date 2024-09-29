#pragma once
#include "AppManager.h"
#include "ui/lookupEditor.h"
#include "ui/terrainEditor.h"
#include "map/Map.h"
#include "AppManager.h"
#include "../thirdparty/lodepng/textureHandler.h"
namespace MapGeneratorTool
{
	ApplicationManager::ApplicationManager()
	{
		m_currentPanel = std::make_unique<ui::LookupEditor>("lookup");
	}

	void ApplicationManager::Init(unsigned width, unsigned height)
	{
		// generate Map
		const int seedsNumber = 10;
		m_map = std::make_unique<Map>(width, height, seedsNumber, "lookupmyGALnew.png");
		m_map->GenerateHeightMap(NoiseMapData(width, height));
		m_map->GenerateLookupMapFromMask(LookupMapData(width, height, 0, seedsNumber, 20), textureHandler::decodeOneStep("terrainMask.png"));

		SetLookupData(LookupMapData(width, height, 0, seedsNumber));
		SetNoiseData(NoiseMapData(width, height));
		SwitchState(State::DiagramEditor);
	}
	void ApplicationManager::SwitchState(State newState)
	{
		switch (newState)
		{
		case State::DiagramEditor:
			m_currentPanel = std::make_unique<ui::LookupEditor>("lookup", m_lookupData);
			break;
		case State::TerrainEditor:
			m_currentPanel = std::make_unique<ui::TerrainEditor>("terrain", m_noiseMapData);
			break;
		default:
			break;
		}

		m_currentState = newState;
	}

	void ApplicationManager::ProcessEvents()
	{
		for (auto& event : EventQueue)
		{
			event->Execute(*m_map);
		}
		EventQueue.clear();
	}
}