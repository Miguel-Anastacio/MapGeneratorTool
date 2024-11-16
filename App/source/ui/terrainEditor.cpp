#pragma once
#include "terrainEditor.h"
#include "../AppManager.h"
namespace MapGeneratorTool
{
namespace ui
{
void TerrainEditor::RenderPanel() const
{
    ImGuiWindowFlags window_flags = SetupWindowFlags();

    ImGui::Begin(m_name, nullptr, window_flags);

    if (ImGui::TreeNode("Noise"))
    {    // height map editing
        bool generate = false;
        static int width = m_data.width;
        generate = generate || ImGui::SliderInt("Width", &width, 10, 2048, "%d");
        static int height = m_data.height;
        generate = generate || ImGui::SliderInt("Height", &height, 10, 2048, "%d");
        static int seed = m_data.seed;
        generate = generate || ImGui::SliderInt("Seed", &seed, -3000, 3000, "%d");
        static int octave = m_data.octaves;
        generate = generate || ImGui::SliderInt("Octaves", &octave, 1, 8, "%d");
        static float frequency = m_data.frequency;
        generate = generate || ImGui::DragFloat("Frequency", &frequency, 0.001f);
        static float scale = m_data.scale;
        generate = generate || ImGui::DragFloat("Scale", &scale, 0.5, -100, 100);
        static float lacunarity = m_data.lacunarity;
        generate = generate || ImGui::DragFloat("Lacunarity", &lacunarity, 0.1f, 1, 5);

        static char text[256] = "terrainNoise.png";
        ImGui::InputText("File Name", text, IM_ARRAYSIZE(text));
        ImGui::SameLine();
        if (ImGui::Button("Save"))
        {
            //ApplicationManager::Get().EventQueue.emplace_back(std::make_unique<SaveEvent>(text, SaveType::Noise));
        }

        if (generate)
        {
            ApplicationManager::Get().EventQueue.emplace_back(std::make_unique<GeneratNoiseMapEvent>(NoiseMapData(width, height, seed, octave, frequency, scale, lacunarity)));
        }

        ImGui::TreePop();
        ImGui::Spacing();
    }

    if(ImGui::TreeNode("Terrain"))
    {  
        renderTerrainEdit();
        ImGui::TreePop();
        ImGui::Spacing();
    }


    
    //if (ImGui::Button("Generate"))
    //{
    //    StateManager::Get().EventQueue.emplace_back(std::make_unique<GeneratNoiseMapEvent>(NoiseMapData(width, height, seed, octave, frequency, scale, lacunarity)));
    //}



    ImGui::End();
}
void TerrainEditor::renderTerrainEdit() const
{
    static std::vector<TerrainType> types;
    bool generate = false;

    if (ImGui::SmallButton("+"))
    {
        //types.emplace_back(TerrainType(0.5, sf::Color::Red, "Red" + types.size()));
        generate = true;
    }

    auto it = types.begin();
    while (it != types.end()) 
    {
        if (!renderTerrainType(generate, *it))
        {
            it = types.erase(it);
        }
        else 
        {
            ++it;
        }
    }

    if (generate)
    {
        ApplicationManager::Get().EventQueue.emplace_back(std::make_unique<GenerateTerrainMapEvent>(types));
    }

    static char textTerrain[256] = "terrainMask.png";
    ImGui::InputText("File Name", textTerrain, IM_ARRAYSIZE(textTerrain));
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        //ApplicationManager::Get().EventQueue.emplace_back(std::make_unique<SaveEvent>(textTerrain, SaveType::Terrain));
    }

}
bool TerrainEditor::renderTerrainType(bool& generate, TerrainType& t) const
{
    if (ImGui::TreeNode(t.name.c_str()))
    {
        ////float minHeight2 = t.minHeight;
        //generate = generate || ImGui::DragFloat("minHeight", &t.minHeight, 0.005f);
        //float col1[3] = { t.color.r / 255.f, t.color.g / 255.f, t.color.b / 255.f };
        //generate = generate || ImGui::ColorEdit3("color 1", col1);
        ////ImGui::SameLine(); HelpMarker(
        ////    "Click on the color square to open a color picker.\n"
        ////    "Click and hold to use drag and drop.\n"
        ////    "Right-click on the color square to show options.\n"
        ////    "CTRL+click on individual component to input value.\n");
        //t.color = sf::Color(255 * col1[0], 255 * col1[1], 255 * col1[2]);

        //// Create a buffer that ImGui can use
        //char buffer[256];
        //std::strncpy(buffer, t.name.c_str(), sizeof(buffer));
        //buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination
        //if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
        //{
        //    t.name = std::string(buffer);
        //}
     
        bool alive = true;
        if (ImGui::SmallButton("-"))
        {
            generate = true;
            alive = false;
        }

        ImGui::TreePop();
        ImGui::Spacing();
        return alive;
    }
    return true;
}
}
}