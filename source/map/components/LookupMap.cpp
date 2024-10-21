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
#include "Algo.h"
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
		auto landTileMap = GenerateTileMapFromMask(data.land, landMask, TileType::LAND, "landMaskLookUp.png");
		auto oceanTileMap = GenerateTileMapFromMask(data.ocean, oceanMask, TileType::WATER, "oceanMaskLookUp.png");

		landMask->Texture().clear();
		oceanMask->Texture().clear();
		rend::drawTileMap(landTileMap, landMask->Texture(), width, height);
		rend::drawTileMap(oceanTileMap, oceanMask->Texture(), width, height);

		TileMap lookUpTileMap = TileMap::BlendTileMap(landTileMap, TileType::LAND, oceanTileMap, TileType::WATER);
		m_texture.clear();
		rend::drawTileMap(lookUpTileMap, m_texture, width, height);

		OutputLookupTable();
	}

	/*std::vector<uint8_t> LookupMap::GenerateLookupMapFromMask(const LookupFeatures& data, const MapMask* mapMask, const char* name)
	{
		const auto width = Width();
		const auto height = Height();
		const auto buffer = mapMask->GetMaskBuffer();

		Mask mask(width, height, buffer);
		auto pointsContr = geomt::generatePointsConstrained<double>(data.tiles, data.seed, true, mask);
		mygal::Diagram<double>  diagram = std::move(geomt::generateDiagram(pointsContr));
		geomt::lloydRelaxation(diagram, data.lloyd);

		TileMap maskTileMap(width, height);

		rasterizer::RasterizeDiagramToTileMap(diagram, width, height, maskTileMap, 40.0f);
		auto tileMap = maskTileMap.GetTilesRef();

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
		algo::floodFill(tileMap, centroids, Width(), Height());


		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto index = y * width + x;
				if (!tileMap[index].visited)
				{
					auto color = FindClosestTileOfSameType(tileMap, x, y, width, height);
					if (color != Utils::Color(0, 0, 0, 0))
						algo::fill(x, y, tileMap, color, width, height);
				}
			}
		}
		return maskTileMap.ConvertTileMapToBuffer();

	}*/

	TileMap LookupMap::GenerateTileMapFromMask(const LookupFeatures& data, const MapMask* mapMask, TileType type, const char* name)
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

		TileMap maskTileMap(width, height);
		rasterizer::RasterizeDiagramToTileMap(diagram, width, height, maskTileMap, 20.0f);
		auto& tileMap = maskTileMap.GetTilesRef();

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (!mask.isInMask(x, y))
				{
					tileMap[y * width + x].visited = true;
				}
				else
				{
					tileMap[y * width + x].type = type;
				}
			}
		}

		std::vector<mygal::Vector2<double>> centroids;
		centroids.reserve(diagram.getSites().size());
		for (auto& site : diagram.getSites())
		{
			centroids.emplace_back(site.point);
		}
		algo::floodFill(tileMap, centroids, Width(), Height());


		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto index = y * width + x;
				if (!tileMap[index].visited)
				{
					auto color = FindClosestTileOfSameType(tileMap, x, y, width, height);
					if (color != Utils::Color(0, 0, 0, 0))
						algo::fill(x, y, tileMap, color, width, height);
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
				if (tileMap[index].isBorder)
				{
					auto color = FindClosestTileOfSameType(tileMap, x, y, width, height);
					if(color != Utils::Color(0, 0, 0, 0))
						tileMap[index].color = color;
						
					//break;
				}
			}
		}

		return maskTileMap;
	}


	Utils::Color LookupMap::FindClosestTileOfSameType(const std::vector<Tile>& tileMap, int x, int y, unsigned width, unsigned height) const
	{
		std::unordered_set<mygal::Vector2<int>> tilesVisited;
		const  int startIdx = y * width + x;
		auto targetType = tileMap[startIdx].type;
		std::stack<std::pair<int, int>> stack;

		// Initialize BFS queue with the starting position
		stack.push({ x, y });

		while (!stack.empty()) 
		{
			auto [cx, cy] = stack.top();
			stack.pop();
			if (cx < 0 || cx >= width || cy < 0 || cy >= height) continue;
			if (tilesVisited.contains(mygal::Vector2<int>(cx, cy))) continue;

			const Tile& tile = tileMap[cy * width + cx];

			if (tile.type == targetType && (tile.visited) && (!tile.isBorder))
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