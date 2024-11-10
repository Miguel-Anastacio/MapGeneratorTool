#pragma once
#include "AppManager.h"
namespace Core
{
	ApplicationManager::ApplicationManager()
	{
		//m_currentPanel = std::make_unique<ui::LookupEditor>("lookup");
		std::srand(time(0));

	}

	void ApplicationManager::Init(unsigned width, unsigned height)
	{
		
		SwitchState(State::DiagramEditor);
	}
	void ApplicationManager::SwitchState(State newState)
	{
		switch (newState)
		{
		case State::DiagramEditor:
			//m_currentPanel = std::make_unique<ui::LookupEditor>("lookup", m_lookupData);
			break;
		case State::TerrainEditor:
			//m_currentPanel = std::make_unique<ui::TerrainEditor>("terrain", m_noiseMapData);
			break;
		default:
			break;
		}

		m_currentState = newState;
	}

	void ApplicationManager::ProcessEvents()
	{
	/*	for (auto& event : EventQueue)
		{
			event->Execute(*m_map);
		}
		EventQueue.clear();*/
	}
}