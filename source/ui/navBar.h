#pragma once
#include "../AppManager.h"
constexpr const int MENUS = 2;
namespace MapGeneratorTool
{
namespace ui
{
	class NavBar
	{
	public:
		void RenderNavBar();
		
	private:
		void renderSelectable(bool& flagToUpdate, const char* text, State state);
	};

}
}