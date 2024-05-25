#pragma once
#include "terrainEditor.h"
#include "imgui.h"
namespace MapGeneratorTool
{
namespace ui
{
	void TerrainEditor::RenderPanel() const
	{
        ImGuiWindowFlags window_flags = SetupWindowFlags();

        ImGui::Begin(m_name, nullptr, window_flags);

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("Hey"))
            {
               
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Test Terrain"))
            {
                ImGui::EndTabItem();
            }
           
            ImGui::EndTabBar();
        }


        ImGui::End();
	}
}
}