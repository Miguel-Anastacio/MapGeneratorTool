#include "navBar.h"
#include <imgui.h>
#include <FileIO.hpp>
#include "Utils.h"
#include "TexturePanel.h"
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
            renderFileMenu();
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
            ApplicationManager::Get().SwitchState(state);
            mainImage = 0;
        }
    }
    void NavBar::renderFileMenu()
    {

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load Height Map")) 
            {
                std::vector<std::wstring> paths;
                FileIO::OpenFile(paths);
                if (!paths.empty())
                {
                    auto string = Utils::WStringToString(paths[0]);
                    ApplicationManager::Get().EventQueue.emplace_back(std::make_unique<LoadHeightMapEvent>(string));
                }
                 
            }
            if (ImGui::MenuItem("Load Generated", "Ctrl+O")) 
            {

            }
            if (ImGui::MenuItem("Save Map Generation", "Ctrl+S"))
            {
                std::wstring path;
                FileIO::SelectDirectory(path);
                auto string = Utils::WStringToString(path);
                ApplicationManager::Get().EventQueue.emplace_back(std::make_unique<SaveEvent>(string));
               
            } 
            if (ImGui::MenuItem("Save current texture", "Ctrl+S+Shift"))
            {

            }

            ImGui::EndMenu();
        }
    }
}
}