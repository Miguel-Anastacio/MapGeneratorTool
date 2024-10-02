#pragma once
#include "MapComponent.h"
#include "Renderer.h"
namespace MapGeneratorTool
{
	void MapComponent::SaveToFile() const
	{
		rend::saveToFile(m_texture, m_name);
	}

	void MapComponent::SaveToFile(const char* filename) const
	{
		rend::saveToFile(m_texture, filename);
	}

	void MapComponent::SaveToFile(const std::string& filepath) const
	{
		std::string name = filepath + "\\" + m_name;
		rend::saveToFile(m_texture, name.c_str());
	}


}