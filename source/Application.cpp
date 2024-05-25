#pragma once
#include "Application.h"
#include "../thirdparty/lodepng/textureHandler.h"
#include "../thirdparty/fastNoiseLite/FastNoiseLite.h"
#include "HeightMap.h"
#include "texture/Texture.h"
#include "map/Map.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include "ui/navBar.h"
#include "ui/basePanel.h"
#include "ui/lookupEditor.h"
#include "ui/terrainEditor.h"

#include "StateManager.h"
namespace MapGeneratorTool
{
	using namespace Utils;

	// Taken from https://github.com/ocornut/imgui/issues/707
	void setFancyImguiStyle() {
		ImGui::GetStyle().FrameRounding = 4.0f;
		ImGui::GetStyle().GrabRounding = 4.0f;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void ProccessWindowEvents(sf::RenderWindow& window, sf::Clock& deltaClock)
	{
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
	}

	void ProcessEvents(Map& map)
	{
		auto& test = StateManager::Get().EventQueue;
		for (auto& event : StateManager::Get().EventQueue)
		{
			event->Execute(map);
		}

		StateManager::Get().EventQueue.clear();
	}

	void RenderState(const sf::RenderTexture& texture, const ui::BasePanel& panel)
	{
		panel.RenderPanel();

		static ImVec2 viewportSize{ 500, 500 };
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 10.f));
		if (ImGui::Begin("Viewport")) {
			viewportSize = ImGui::GetWindowSize();
			/*sf::Vector2u size = texture.getSize();
			if (size.x > 1024)
			{
				size.x = 1024;
			}
			if (size.y > 1024)
			{
				size.y = 1024;
			}*/

			ImGui::Image(texture);
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}


	void Run()
	{
		// INIT
		sf::RenderWindow window(sf::VideoMode(1280, 720), "MapGeneratorTool");
		window.setFramerateLimit(60);
		ImGui::SFML::Init(window);
		// enable docking
		auto& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		setFancyImguiStyle();

		ui::NavBar nav;

		// generate Map
		const unsigned width = 1024, height = 1024;
		const int seedsNumber = 400;
		Map newMap = Map(width, height, seedsNumber, "lookupmyGALnew.png");
		StateManager::Get().SetLookupData(LookupMapData(width, height, 0, seedsNumber));
		StateManager::Get().SwitchState(State::DiagramEditor);

		sf::Clock deltaClock;
		while (window.isOpen()) 
		{
			ProccessWindowEvents(window, deltaClock);
			ProcessEvents(newMap);
			ImGui::DockSpaceOverViewport();

			nav.RenderNavBar();
			RenderState(newMap.lookupTexture(), *StateManager::Get().CurrentPanel());

			window.clear();
			ImGui::SFML::Render(window);
			window.display();
		}

		ImGui::SFML::Shutdown();
	}


	void GenerateStuff()
	{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		const unsigned width = 1024, height = 1024;
		const int seedsNumber = 400;
		//FastNoiseLite noise;
		//noise.SetSeed(100123);
		//noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		//noise.SetFrequency(0.01f);
		//noise.SetFractalOctaves(5);
		//noise.SetFractalType(FastNoiseLite::FractalType_FBm);
		//noise.SetFractalLacunarity(2.0f);
		//noise.SetFractalGain(0.5f);
		//HeightMap map1 = HeightMap(width, height, "heightMap1.png", 1, noise, NoiseSpecs(1, 1, 1));
		//HeightMap map = HeightMap(width, height, "TestHM- Lacunarity2.png", 302.46, perlin, NoiseSpecs(1, 1, 1));
		//HeightMap map2 = HeightMap(width, height, "TestHM3-Lacunarity16.png", 27.6, perlin, NoiseSpecs(16, 0.5, 8));
		//HeightMap map3 = HeightMap(width, height, "TestHM4Lacunarity32.png", 27.6, perlin, NoiseSpecs(32, 0.5, 3));
		//MapGeneratorTool::textureHandler::encodeOneStep("heightMap.png", image, width, height); 
		//map1.SaveHeightMapToFile();s
		//map1.SaveHeightMapToFile();
		//map2.SaveHeightMapToFile();
		//map3.SaveHeightMapToFile();

		Map newMap = Map(width, height, seedsNumber, "lookupmyGAL.png");
		//std::vector<Vector2> points = FortuneAlgo::generatePoints(seedsNumber);
		//VoronoiDiagram diagram = FortuneAlgo::generateRandomDiagram(seedsNumber);
		//FortuneAlgo::drawDiagram(diagram);
		//Map mapWithMask = Map("mask.png", seedsNumber, "lookup3mask.png");


		/*auto points = generatePoints<double>(1000);
		mygal::Diagram diagram = generateDiagram(points);
		auto polygons = diagram.GetPolygons();*/

	}


}