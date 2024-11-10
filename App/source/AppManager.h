#pragma once
#include <memory>
#include <vector>
#include "ui/basePanel.h"
#include "Event.h"
/*
*	Controls the application workflow, handles menus transitions 
*	Connects the UI with the map creation logic 
*	Handles user events 
*/

namespace MapGeneratorTool
{
	enum class State
	{
		DiagramEditor,
		TerrainEditor
	};

	class ApplicationManager
	{
	public:
		static ApplicationManager& Get()
		{
			static ApplicationManager instance;
			return instance;
		}

		void Init(unsigned width = 1024, unsigned height = 512);

		void SwitchState(State newState);

		inline void SetLookupData(const LookupMapData& data) {
			m_lookupData = data;
		}
		inline void SetNoiseData(const NoiseMapData& data) {
			m_noiseMapData = data;
		}

		inline State CurrentState() const {
			return m_currentState;
		}

		inline ui::BasePanel* CurrentPanel() const
		{
			return m_currentPanel.get();
		}

		void ProcessEvents();

		inline const Map& GetMap() const
		{
			return *(m_map.get());
		}

		std::vector<std::unique_ptr<Event>> EventQueue;

	private:
		ApplicationManager();
		~ApplicationManager() = default;
		ApplicationManager(ApplicationManager& other) = delete;
		void operator = (const ApplicationManager&) = delete;
		ApplicationManager(ApplicationManager&&) = delete;

		State m_currentState;
		std::unique_ptr<ui::BasePanel> m_currentPanel;
		LookupMapData m_lookupData;
		NoiseMapData m_noiseMapData;

		std::unique_ptr<Map> m_map;
	};

}