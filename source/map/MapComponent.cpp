#pragma once
#include "MapComponent.h"
#include "../utils/Renderer.h"
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


}