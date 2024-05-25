#pragma once
#include "basePanel.h"
namespace MapGeneratorTool
{
namespace ui
{
	class TerrainEditor : public BasePanel
	{
	public:
		TerrainEditor(const char* name) : BasePanel(name) {};
		void RenderPanel() const override;

	private:

	};

}
}