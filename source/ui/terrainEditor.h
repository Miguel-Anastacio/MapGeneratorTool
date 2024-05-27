#pragma once
#include "basePanel.h"
#include "../map/MapSpecs.h"
namespace MapGeneratorTool
{
namespace ui
{
	class TerrainEditor : public BasePanel
	{
	public:
		TerrainEditor(const char* name) : BasePanel(name) {};
		TerrainEditor(const char* name, const NoiseMapData& data) : BasePanel(name), m_data(data) {};
		void RenderPanel() const override;
		//void SetMapProperties(int width, int height, int seed, int numberOfSeed, int lloyd);
		inline void SetMapProperties(const NoiseMapData& data) {
			m_data = data;
		}

	private:
		NoiseMapData m_data;
	};

}
}