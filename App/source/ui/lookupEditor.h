#pragma once
#include "GUI/basePanel.h"
#include "map/MapSpecs.h"
#include <functional>
namespace MapGeneratorTool
{
namespace ui
{
	class LookupEditor : public GUI::BasePanel
	{
		//using CallBack = std::function<void()>;
		using LookupMapData = MapGenerator::LookupMapData;
	public:
		LookupEditor(const char* name) : BasePanel(name) {};
		LookupEditor(const char* name, const LookupMapData& data) : BasePanel(name), m_data(data) {};
		void RenderPanel() const override;
		void SetMapProperties(int width, int height, int seed, int numberOfSeed, int lloyd);
		inline void SetMapProperties(const LookupMapData& data) {
			m_data = data;
		}
	private:
		
		LookupMapData m_data;
		//CallBack m_callback;
	};

}
}