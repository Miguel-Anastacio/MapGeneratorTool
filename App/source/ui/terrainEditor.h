#pragma once
#include "GUI/basePanel.h"
//#include "map/MapSpecs.h"
#include "utils/Event.h"
namespace MapGeneratorTool
{
namespace ui
{
	class TerrainEditor : public GUI::BasePanel
	{
		using NoiseMapData = MapGenerator::NoiseMapData;
		using TerrainType = MapGenerator::TerrainType;
	public:
		TerrainEditor(const char* name) : BasePanel(name) {};
		TerrainEditor(const char* name, const NoiseMapData& data) : BasePanel(name), m_data(data) {};
		void RenderPanel() const override;
	
	private:

		void renderTerrainEdit() const;
		bool renderTerrainType(bool& generate, TerrainType& t) const;

		
		NoiseMapData m_data;
	};

}
}