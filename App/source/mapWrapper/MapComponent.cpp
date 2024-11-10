#pragma once
#include "MapComponent.h"
#include "graphics/Renderer.h"
namespace MapGeneratorTool
{
	void MapComponentSFML::SaveToFile() const
	{
		rend::saveToFile(m_texture, m_mapData.Name());
	}

	void MapComponentSFML::SaveToFile(const char* filename) const
	{
		rend::saveToFile(m_texture, filename);
	}

	void MapComponentSFML::SaveToFile(const std::string& filepath) const
	{
		std::string name = filepath + "\\" + m_mapData.Name();
		rend::saveToFile(m_texture, name.c_str());
	}


}