#pragma once
#include "lookupEditor.h"
#include "imgui.h"
namespace MapGeneratorTool
{
namespace ui
{
	void LookupEditor::RenderPanel() const
	{
        ImGuiWindowFlags window_flags = SetupWindowFlags();

        ImGui::Begin(m_name, nullptr, window_flags);

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("Insert File Name"))
            {
               
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Drag and Drop File"))
            {
                ImGui::EndTabItem();
            }
           
            ImGui::EndTabBar();
        }


        ImGui::End();
	}
}
}