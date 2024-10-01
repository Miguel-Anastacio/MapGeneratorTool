#pragma once
#include <algorithm>
#include "../../thirdparty/MyGAL/Vector2.h"
#include "Renderer.h"
#include "components/LookupMap.h"
#include "components/MapMask.h"
#include "geometry/ComputeGeometry.h"
#include "Mask.h"
namespace MapGeneratorTool
{

	LookupMap::LookupMap(const char* name, unsigned width, unsigned height)
	: MapComponent(width, height, name)
	{

	}

	void LookupMap::RegenerateLookUp(const LookupMapData& data, MapMask* landMask, MapMask* oceanMask)
	{
		const auto width = Width();
		const auto height = Height();
		m_colorsInUse.clear();
		std::vector<uint8_t> llmBuffer = GenerateLookupMapFromMask(data.land, landMask, "landMaskLookUp.png");
		std::vector<uint8_t> lloBuffer = GenerateLookupMapFromMask(data.ocean, oceanMask, "oceanMaskLookUp.png");

		landMask->Texture().clear();
		oceanMask->Texture().clear();
		rend::drawBuffer(llmBuffer, landMask->Texture(), width, height);
		rend::drawBuffer(lloBuffer, oceanMask->Texture(), width, height);

		std::vector<uint8_t> temp(width * height * 4);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				unsigned int index = (y * width + x) * 4;

				if (llmBuffer[index + 3] > lloBuffer[index + 3])
				{
					temp[index + 0] = llmBuffer[index];
					temp[index + 1] = llmBuffer[index + 1];
					temp[index + 2] = llmBuffer[index + 2];
					temp[index + 3] = llmBuffer[index + 3];
				}
				else
				{
					temp[index + 0] = lloBuffer[index];
					temp[index + 1] = lloBuffer[index + 1];
					temp[index + 2] = lloBuffer[index + 2];
					temp[index + 3] = lloBuffer[index + 3];
				}
			}
		}

		m_texture.clear();
		rend::drawBuffer(temp, m_texture, width, height);
		//rend::drawPoints(m_lookupTexture, diagram, data.width, data.height);
		//rend::saveToFile(m_texture, "finalLookup.png");
	}

	std::vector<uint8_t> LookupMap::GenerateLookupMapFromMask(const LookupFeatures& data, const MapMask* mapMask, const char* name)
	{
		const auto width = Width();
		const auto height = Height();
		const auto buffer = mapMask->GetMaskBuffer();

		Mask mask(width, height, buffer);
		auto pointsContr = geomt::generatePointsConstrained<double>(data.tiles, data.seed, true, mask);
		mygal::Diagram<double>  diagram = std::move(geomt::generateDiagram(pointsContr));
		geomt::lloydRelaxation(diagram, data.lloyd);

		// write diagram to image to get a pixel array
		sf::RenderTexture texture;
		texture.clear();
		texture.create(width, height);
		rend::drawPolygons(diagram.GetPolygons(), texture, width, height, m_colorsInUse);
		sf::Image image = texture.getTexture().copyToImage();

		// Get the pixel array (RGBA values)
		const sf::Uint8* pixels = image.getPixelsPtr();

		std::vector<uint8_t> temp(width * height * 4);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				unsigned int index = (y * width + x) * 4;
				temp[index + 0] = pixels[index] * (buffer[index + 0] / 255);
				temp[index + 1] = pixels[index + 1] * (buffer[index + 1] / 255);
				temp[index + 2] = pixels[index + 2] * (buffer[index + 2] / 255);
				temp[index + 3] = pixels[index + 3] * (buffer[index + 3] / 255);
			}
		}
		//rend::drawPoints(m_lookupTexture, diagram, data.width, data.height);
		//rend::saveToFile(texture, name);


		return temp;

	}

}