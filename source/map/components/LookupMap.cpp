#pragma once
#include <algorithm>
#include "../../thirdparty/MyGAL/Vector2.h"
#include "Renderer.h"
#include "components/LookupMap.h"
#include "components/MapMask.h"
#include "geometry/ComputeGeometry.h"
#include "Mask.h"
#include "json.hpp"
#include <fstream> 
#include <queue>
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
		//OutputLookupTable();


		//CreateLookupWithTileMap(data, landMask, oceanMask);
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

		auto tileMap = rasterizer::CreateTileFromDiagram(diagram, Width(), Height(), 20.0f);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				tileMap[y * width + x].land = mask.isInMask(x, y);

				if (!mask.isInMask(x, y))
					tileMap[y * width + x].visited = true;
			}
		}

		std::vector<mygal::Vector2<double>> centroids;
		centroids.reserve(diagram.getSites().size());
		for (auto& site : diagram.getSites())
		{
			centroids.emplace_back(site.point);
		}
		rend::floodFill(tileMap, centroids, Width(), Height());


		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto index = y * width + x;
				if (!tileMap[index].visited)
				{
					auto color = FindClosestTileOfSameType(tileMap, x, y, width, height);
					if (color != Utils::Color(0, 0, 0, 0))
						rend::fill(x, y, tileMap, color, width, height);
					//break;
					//tileMap[index].visited = true;
				}
			}
		}

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto index = y * width + x;
				if (!tileMap[index].visited)
				{
					tileMap[index].color = Utils::Color(255, 0, 0, 255);
				}
			}
		}

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto index = y * width + x;
				if (tileMap[index].isBorder)
				{
					auto color = FindClosestTileOfSameType(tileMap, x, y, width, height);
					if(color != Utils::Color(0, 0, 0, 0))
						rend::fill(x, y, tileMap, color, width, height);
					//break;
					//tileMap[index].visited = true;
				}
			}
		}

		rend::drawTileMap(texture, tileMap, width, height);
		//rend::drawPolygons(diagram.GetPolygons(), texture, width, height, m_colorsInUse);

		//rend::drawPolygonsByBuffer(diagram.GetPolygons(), texture, width, height, m_colorsInUse);
		//sf::Image image = texture.getTexture().copyToImage();

		//// Get the pixel array (RGBA values)
		//const sf::Uint8* pixels = image.getPixelsPtr();

		//std::vector<uint8_t> temp(width * height * 4);
		//for (int y = 0; y < height; y++)
		//{
		//	for (int x = 0; x < width; x++)
		//	{
		//		unsigned int index = (y * width + x) * 4;
		//		temp[index + 0] = pixels[index] * (buffer[index + 0] / 255);
		//		temp[index + 1] = pixels[index + 1] * (buffer[index + 1] / 255);
		//		temp[index + 2] = pixels[index + 2] * (buffer[index + 2] / 255);
		//		temp[index + 3] = pixels[index + 3] * (buffer[index + 3] / 255);
		//	}
		//}
		//rend::drawPoints(m_lookupTexture, diagram, data.width, data.height);
		//rend::saveToFile(texture, name);


		return ConvertTileMapToBuffer(tileMap);

	}



	void LookupMap::CreateLookupWithTileMap(const LookupMapData& data, MapMask* landMask, MapMask* oceanMask)
	{
		const auto width = Width();
		const auto height = Height();

		const auto buffer = landMask->GetMaskBuffer();
		Mask mask(width, height, buffer);
		auto points = geomt::generatePointsConstrained<double>(data.land.tiles, data.land.seed, true, mask);
		mygal::Diagram<double>  diagramLand = std::move(geomt::generateDiagram(points));
		geomt::lloydRelaxation(diagramLand, data.land.lloyd);

		const auto buffer1 = oceanMask->GetMaskBuffer();
		Mask maskOcean(width, height, buffer1);
		auto pointsOcean = geomt::generatePointsConstrained<double>(data.ocean.tiles, data.ocean.seed, true, maskOcean);
		mygal::Diagram<double>  diagramOcean = std::move(geomt::generateDiagram(pointsOcean));
		geomt::lloydRelaxation(diagramOcean, data.ocean.lloyd);

		auto landTileMap = rasterizer::CreateTileFromDiagram(diagramLand, Width(), Height(), 1.0f);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				landTileMap[y * width + x].land = mask.isInMask(x, y);
				
			}
		}

		auto oceanTileMap = rasterizer::CreateTileFromDiagram(diagramOcean, Width(), Height(), 1.0f);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				landTileMap[y * width + x].land = maskOcean.isInMask(x, y);

			}
		}

		std::vector<mygal::Vector2<double>> centroids;
		centroids.reserve(diagramOcean.getSites().size());
		for (auto& site : diagramOcean.getSites())
		{
			centroids.emplace_back(site.point);
		}
		centroids.reserve(diagramLand.getSites().size());
		for (auto& site : diagramLand.getSites())
		{
			centroids.emplace_back(site.point);
		}

		rend::floodFill(landTileMap, centroids, width, height);

	/*	for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto index = y * width + x;
				if (tileMap[index].isBorder)
				{
					auto centroid = tileMap[index].centroid;
					tileMap[index].color = tileMap[centroid.y * width + centroid.x].color;
				}
			}
		}

		rend::drawTileMap(m_texture, tileMap, width, height);*/


	/*	rend::floodFill(tileMap, centroids, width, height);

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto index = y * width + x;
				if (tileMap[index].isBorder)
				{
					auto centroid = tileMap[index].centroid;
					tileMap[index].color = tileMap[centroid.y * width + centroid.x].color;
				}
			}
		}*/

		//rend::drawTileMap(m_texture, tileMap, width, height);
	}

	void LookupMap::GenerateTileMap(const LookupFeatures& data, const MapMask* mask, std::vector<rasterizer::Tile>& tileMap, const char* name)
	{
		const auto width = Width();
		const auto height = Height();
		const auto buffer = mask->GetMaskBuffer();

		Mask maskData(width, height, buffer);
		auto pointsContr = geomt::generatePointsConstrained<double>(data.tiles, data.seed, true, maskData);
		mygal::Diagram<double>  diagram = std::move(geomt::generateDiagram(pointsContr));
		geomt::lloydRelaxation(diagram, data.lloyd);





	}

	Utils::Color LookupMap::FindClosestTileOfSameType(const std::vector<rasterizer::Tile>& tileMap, int x, int y, unsigned width, unsigned height) const
	{
		std::unordered_set<mygal::Vector2<int>> tilesVisited;
		const  int startIdx = y * width + x;
		auto targetType = tileMap[startIdx].land;
		std::stack<std::pair<int, int>> stack;

		// Initialize BFS queue with the starting position
		stack.push({ x, y });

		while (!stack.empty()) 
		{
			auto [cx, cy] = stack.top();
			stack.pop();
			if (cx < 0 || cx >= width || cy < 0 || cy >= height) continue;
			if (tilesVisited.contains(mygal::Vector2<int>(cx, cy))) continue;

			const rasterizer::Tile& tile = tileMap[cy * width + cx];

			if (tile.land == targetType && (tile.visited) && (!tile.isBorder))
				return tile.color;
			else
				tilesVisited.insert(mygal::Vector2<int>(cx, cy));

			stack.push({ cx + 1, cy });
			stack.push({ cx - 1, cy });
			stack.push({ cx, cy + 1 });
			stack.push({ cx, cy - 1 });

			
		}

		return Utils::Color();
	}

	void LookupMap::OutputLookupTable() const
	{
		/// print a json file whit an id associated with a colour
		nlohmann::ordered_json jsonArray = nlohmann::ordered_json::array();

		int i = 0;
		for (auto color : m_colorsInUse)
		{
			jsonArray.push_back({ {"Name", i}, {"Color", color.ConvertToHex()}});
			i++;
		}
		//std::cout << jsonArray.dump(4) << std::endl;

		// Write the JSON array to a file
		std::ofstream outFile("output.json"); // Open file in write mode
		if (outFile.is_open()) {
			outFile << jsonArray.dump(4);  // Write to file with indentation
			outFile.close();  // Close the file after writing
		}
		else {
			std::cerr << "Could not open the file for writing!" << std::endl;
		}
	}

}