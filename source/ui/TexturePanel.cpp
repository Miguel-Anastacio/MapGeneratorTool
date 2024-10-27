#pragma once
#include "TexturePanel.h"
#include "imgui-SFML.h"
#include <iostream>
#include <algorithm>
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
			// Set up a horizontal layout for large and small images
			ImGui::BeginChild("Large Image", ImVec2(850, 0), true); // Left side for the large image

			ImVec2 imageSize(800, 600);
			static float zoom = 1;
			static ImVec2 panOffset(0, 0);
			HandleMouseInput(zoom, panOffset, imageSize);

			// Define UV coordinates for zooming and panning
			ImVec2 uv0 = ImVec2(0.5f, 0.5f) - ImVec2(0.5f, 0.5f) / zoom + panOffset;
			ImVec2 uv1 = ImVec2(0.5f, 0.5f) + ImVec2(0.5f, 0.5f) / zoom + panOffset;

			// Clamp UV coordinates to ensure they remain within [0, 1]
			uv0 = clamp(uv0, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
			uv1 = clamp(uv1, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

			// flip UVs Y due to sf::RenderTexture storage
			std::swap(uv0.y, uv1.y);


			// Display the large image
			auto image = (ImTextureID)(intptr_t) (*images[mainImage]).getTexture().getNativeHandle();
			ImVec2(0, 1),
			ImVec2(1, 0), // flipped vertically, because textures in sf::RenderTexture are
				// stored this way
			ImGui::Image(image, imageSize, uv0, uv1); 
			ImGui::EndChild();

			// Place the small images next to the large image
			ImGui::SameLine(); 
			ImGui::BeginChild("Small Images", ImVec2(0, 0), true);

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

	void TexturePanel::HandleMouseInput(float& zoom, ImVec2& panOffset, const ImVec2& imageSize) const
	{
		ImGuiIO& io = ImGui::GetIO();
		const float zoomSpeed = 0.1 * zoom;
		const float maxZoom = 15;
		const float panSpeed = 1.0f;

		// Handle zooming with mouse scroll
		if (ImGui::IsWindowHovered()) 
		{
			float mouse_scroll = io.MouseWheel;
			if (mouse_scroll != 0.0f) {
				// Adjust zoom level based on scroll
				float old_zoom = zoom;

				// Change zoom with scroll, zooming in for positive scroll and out for negative
				zoom += mouse_scroll * zoomSpeed;

				// Clamp zoom level to prevent it from getting too small or large
				zoom = std::clamp(zoom, 1.0f, maxZoom);

				// Adjust the pan offset so that the zooming keeps the image centered under the mouse position
				if (zoom != old_zoom && zoom - old_zoom > 0)
				{
					ImVec2 mouse_pos_in_window = io.MousePos - ImGui::GetCursorScreenPos();
					ImVec2 mouse_uv = mouse_pos_in_window / imageSize;
					ImVec2 zoom_adjustment = (mouse_uv - ImVec2(0.5f, 0.5f)) * (1.0f - (old_zoom / zoom));
					zoom_adjustment.y *= -1;
					panOffset += zoom_adjustment;
				}
				



			}

			// Handle panning when dragging with left mouse button
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) 
			{
				// Update the pan offset when dragging, inverted to match flipped UVs
				ImVec2 drag_delta = io.MouseDelta / imageSize;
				panOffset -= ImVec2(drag_delta.x, -drag_delta.y) / zoom;  // Invert y movement
			}

		}
		
		ImVec2 uv_half_size = ImVec2(0.5f, 0.5f) / zoom;
		// Clamp the pan offset to prevent going beyond the edges of the image
		ImVec2 max_offset = ImVec2(0.5f, 0.5f) - uv_half_size;
		ImVec2 min_offset = ImVec2(-0.5f, -0.5f) + uv_half_size;

		// Apply clamping to ensure the pan offset doesn't exceed the image boundaries
		panOffset = clamp(panOffset, min_offset, max_offset);
	}

}
}