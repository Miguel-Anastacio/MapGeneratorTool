#pragma once
#include <memory>
#include <vector>
#include "ui/basePanel.h"
#include "utils/Event.h"
namespace MapGeneratorTool
{
	enum class State
	{
		DiagramEditor,
		TerrainEditor
	};

	class StateManager
	{
	public:
		static StateManager& Get()
		{
			static StateManager instance;
			return instance;
		}

		void SwitchState(State newState);

		inline void SetLookupData(const LookupMapData& data) {
			m_lookupData = data;
		}

		inline State CurrentState() const {
			return m_currentState;
		}

		inline ui::BasePanel* CurrentPanel() const
		{
			return m_currentPanel.get();
		}

		std::vector<std::unique_ptr<Event>> EventQueue;

	private:
		StateManager();
		~StateManager() = default;
		StateManager(StateManager& other) = delete;
		void operator = (const StateManager&) = delete;
		StateManager(StateManager&&) = delete;

		State m_currentState;
		std::unique_ptr<ui::BasePanel> m_currentPanel;
		LookupMapData m_lookupData;
	};

}