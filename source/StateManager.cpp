#pragma once
#include "StateManager.h"
#include "ui/lookupEditor.h"
#include "ui/terrainEditor.h"
#include "map/Map.h"
namespace MapGeneratorTool
{
	
	StateManager::StateManager()
	{
		m_currentPanel = std::make_unique<ui::LookupEditor>("lookup");
	}
	void StateManager::SwitchState(State newState)
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
}