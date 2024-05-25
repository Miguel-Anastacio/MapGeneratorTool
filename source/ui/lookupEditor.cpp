#pragma once
#include "lookupEditor.h"
#include "imgui.h"
#include "../StateManager.h"
namespace MapGeneratorTool
{
namespace ui
{
void LookupEditor::RenderPanel() const
{
    ImGuiWindowFlags window_flags = SetupWindowFlags();

    ImGui::Begin(m_name, nullptr, window_flags);

    ImGuiSliderFlags_ flags;
   

    static int width = m_data.width;
    ImGui::SliderInt("Width", &width, 10, 2048, "%d");
    static int height = m_data.height;
    ImGui::SliderInt("Height", &height, 10, 2048, "%d");
    static int seed = m_data.seed;
    ImGui::SliderInt("Seed", &seed, -3000, 3000, "%d");
    static int numberOfSeeds = m_data.numberOfSeeds;
    ImGui::SliderInt("Number of Seeds", &numberOfSeeds, 5, 500, "%d");
    static int lloyd = m_data.lloyd;
    ImGui::SliderInt("Lloyd", &lloyd, 0, 20, "%d");


    if (ImGui::Button("Generate"))
    {
        StateManager::Get().EventQueue.emplace_back(std::make_unique<GenerateLookupMapEvent>(LookupMapData(width, height, seed, numberOfSeeds, lloyd)));
    }
    static char text[256] = "test.png";
    ImGui::InputText("File Name", text, IM_ARRAYSIZE(text));
    if (ImGui::Button("Save"))
    {
        StateManager::Get().EventQueue.emplace_back(std::make_unique<SaveMapEvent>(text));
    }

    ImGui::End();
}
void LookupEditor::SetMapProperties(int width, int height, int seed, int numberOfSeed, int lloyd)
{
    m_data.width = width;
    m_data.height = height;
    m_data.seed = seed;
    m_data.numberOfSeeds = numberOfSeed;
    m_data.lloyd = lloyd;
}


}
}