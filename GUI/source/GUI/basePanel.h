#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include "windowSettings.h"
namespace Core
{
namespace GUI
{
	class BasePanel
	{
	public:
		BasePanel(const char* name) : m_name(name) {};
		virtual ~BasePanel() = default;

		virtual void RenderPanel() const = 0;

	protected:
		const char* m_name;

		inline ImGuiWindowFlags SetupWindowFlags() const
		{
			ImGuiWindowFlags window_flags = 0;
			if (NO_MOVE)            window_flags |= ImGuiWindowFlags_NoMove;
			if (NO_RESIZE)          window_flags |= ImGuiWindowFlags_NoResize;
			if (NO_COLLAPSE)        window_flags |= ImGuiWindowFlags_NoCollapse;

			return window_flags;
		}
	};

}
}