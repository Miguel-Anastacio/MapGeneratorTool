#pragma once
#include "terrainEditor.h"
#include "imgui.h"
#include "../StateManager.h"
#include "../utils/Event.h"
namespace MapGeneratorTool
{
namespace ui
{
void TerrainEditor::RenderPanel() const
{
    ImGuiWindowFlags window_flags = SetupWindowFlags();

    ImGui::Begin(m_name, nullptr, window_flags);
    // height map editing

    bool generate = false;
    static int width = m_data.width;
    generate = generate || ImGui::SliderInt("Width", &width, 10, 2048, "%d");
    static int height = m_data.height;
    generate = generate || ImGui::SliderInt("Height", &height, 10, 2048, "%d");
    static int seed = m_data.seed;
    generate = generate || ImGui::SliderInt("Seed", &seed, -3000, 3000, "%d");
    static int octave = m_data.octaves;
    generate = generate || ImGui::SliderInt("Octaves", &octave, 0, 100, "%d");
    static float frequency = m_data.frequency;
    generate = generate || ImGui::DragFloat("Frequency", &frequency, 0.1f, -1.0, 1.0f);
    static float scale = m_data.scale;
    generate = generate || ImGui::SliderFloat("Scale", &scale, -100, 100, "%f");
    static float lacunarity = m_data.lacunarity;
    generate = generate || ImGui::SliderFloat("Lacunarity", &lacunarity, 1.0, 5.0, "%f");

    //if (ImGui::Button("Generate"))
    //{
    //    StateManager::Get().EventQueue.emplace_back(std::make_unique<GeneratNoiseMapEvent>(NoiseMapData(width, height, seed, octave, frequency, scale, lacunarity)));
    //}

    static char text[256] = "test.png";
    ImGui::InputText("File Name", text, IM_ARRAYSIZE(text));
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        StateManager::Get().EventQueue.emplace_back(std::make_unique<SaveNoiseMapEvent>(text));
    }

    if (generate)
    {
        StateManager::Get().EventQueue.emplace_back(std::make_unique<GeneratNoiseMapEvent>(NoiseMapData(width, height, seed, octave, frequency, scale, lacunarity)));
    }

    ImGui::End();
}
}
}