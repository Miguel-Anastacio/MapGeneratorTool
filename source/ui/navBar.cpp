#include "navBar.h"
#include <imgui.h>
namespace MapGeneratorTool
{
namespace ui
{	
	void NavBar::RenderNavBar()
	{
		bool m_selection[MENUS] = { false };
        if (ImGui::BeginMainMenuBar())
        {
            renderSelectable(m_selection[0], " Lookup Editor ", State::DiagramEditor);
            renderSelectable(m_selection[1], " Terrain Editor ", State::TerrainEditor);
            ImGui::EndMainMenuBar();
        }
	}

    void NavBar::renderSelectable(bool& flagToUpdate, const char* text, State state)
    {
        ImGuiSelectableFlags_ flag = ImGuiSelectableFlags_AllowDoubleClick;
        ImVec2 size = ImGui::CalcTextSize(text);
        size.y = size.y * 1.5;
        if (ImGui::Selectable(text, flagToUpdate, flag, size))
        {
            StateManager::Get().SwitchState(state);
        }
    }
}
}