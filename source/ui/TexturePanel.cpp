#pragma once
#include "TexturePanel.h"
#include "imgui-SFML.h"
#include <iostream>
namespace MapGeneratorTool
{
namespace ui
{
	void TexturePanel::RenderPanel() const
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.f, 20.f));
		static ImVec2 viewportSize{ 500, 500 };
		if (ImGui::Begin("Viewport")) 
		{
			auto images = texturesStack;
			ImVec2 windowSize(400, 300);
			// Set up a horizontal layout for large and small images
			ImGui::BeginChild("Large Image", ImVec2(850, 0), true); // Left side for the large image
			ImGui::Image(*images[mainImage], ImVec2(800, 600)); // Display the large image
			ImGui::EndChild();

			ImGui::SameLine(); // Place the small images next to the large image
			ImGui::BeginChild("Small Images", ImVec2(0, 0), true); // Right side for small images

			// Display the small images as buttons
			for (int i = 0; i < images.size(); i++)
			{
				if (ImGui::ImageButton(("ban" + std::to_string(i)).c_str(), *images[i], ImVec2(100, 100)))
				{
					mainImage = i;
				}
			}

			ImGui::EndChild(); // End the small images child
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

}
}