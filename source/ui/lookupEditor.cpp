#pragma once
#include "lookupEditor.h"
#include "imgui.h"
#include "../AppManager.h"
namespace MapGeneratorTool
{
namespace ui
{
void LookupEditor::RenderPanel() const
{
    ImGuiWindowFlags window_flags = SetupWindowFlags();

    ImGui::Begin(m_name, nullptr, window_flags);

    ImGuiSliderFlags_ flags;
    static LookupMapData data = m_data;
    if (ImGui::TreeNode("General Settings"))
    {
        static int width = m_data.width;
        ImGui::SliderInt("Width", &width, 10, 2048, "%d");
        static int height = m_data.height;
        ImGui::SliderInt("Height", &height, 10, 2048, "%d");
        ImGui::SliderFloat("Cutoff height", &data.cutOffHeight, 0.0f, 1.0f);

        data.width = width;
        data.height = height;
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Land Settings"))
    {
        ImGui::SliderInt("Seed", &data.land.seed, 0, 400000, "%d");
        ImGui::SliderInt("Number of Tiles", &data.land.tiles, 0, 500, "%d");
        ImGui::SliderInt("Lloyd Iterations ", &data.land.lloyd, 0, 20);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Ocean Settings"))
    {
        ImGui::SliderInt("Seed", &data.ocean.seed, 0, 400000, "%d");
        ImGui::SliderInt("Number of Tiles", &data.ocean.tiles, 0, 500, "%d");
        ImGui::SliderInt("Lloyd Iterations ", &data.ocean.lloyd, 0, 20);
        ImGui::TreePop();
    }


    if (ImGui::Button("Generate"))
    {
        ApplicationManager::Get().EventQueue.emplace_back(std::make_unique<GenerateLookupMapEvent>(data));
    }
    static char text[256] = "test.png";
    ImGui::InputText("File Name", text, IM_ARRAYSIZE(text));
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        ApplicationManager::Get().EventQueue.emplace_back(std::make_unique<SaveEvent>(text, SaveType::Lookup));
    }

    ImGui::End();
}
void LookupEditor::SetMapProperties(int width, int height, int seed, int numberOfSeed, int lloyd)
{
    m_data.width = width;
    m_data.height = height;
    //m_data.seed = seed;
    //m_data.numberOfSeeds = numberOfSeed;
    //m_data.lloyd = lloyd;
}


} // namespase ui
} // namespace MapGeneratorTool