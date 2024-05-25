#pragma once
#include "basePanel.h"
namespace MapGeneratorTool
{
namespace ui
{
	class LookupEditor : public BasePanel
	{
	public:
		LookupEditor(const char* name) : BasePanel(name) {};
		void RenderPanel() const override;

	private:

	};

}
}