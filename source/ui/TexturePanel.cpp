#pragma once
#include "TexturePanel.h"
#include "imgui-SFML.h"
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
			ImVec2 windowSize = ImGui::GetContentRegionAvail();

			// Calculate half the width of the window
			float halfWidth = windowSize.x / texturesStack.size();
			float height = windowSize.y / texturesStack.size();

			for (auto image : texturesStack)
			{
				//ImGui::Image(*image, windowSize);
				ImGui::Image(*image, ImVec2(halfWidth, height));
				ImGui::Spacing();
				ImGui::Spacing();
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

}
}