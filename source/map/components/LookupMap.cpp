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
#include "Color.h"
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
		auto landTileMap = GenerateTileMapFromMask(data.land, data.borderNoise, data.borderLine, landMask, TileType::LAND, "landMaskLookUp.png");
		auto oceanTileMap = GenerateTileMapFromMask(data.ocean, data.borderNoise, data.borderLine, oceanMask, TileType::WATER, "oceanMaskLookUp.png");

		landMask->Texture().clear();
		oceanMask->Texture().clear();
		rend::drawTileMap(landTileMap, landMask->Texture(), width, height);
		rend::drawTileMap(oceanTileMap, oceanMask->Texture(), width, height);

		TileMap lookUpTileMap = TileMap::BlendTileMap(landTileMap, TileType::LAND, oceanTileMap, TileType::WATER);
		m_texture.clear();
		rend::drawTileMap(lookUpTileMap, m_texture, width, height);
		
		auto centroids = lookUpTileMap.GetCentroids();
		for (const auto point : centroids)
		{
			sf::Color color;
			if (lookUpTileMap.IsTileOfType(TileType::LAND, point.x, point.y))
				color = sf::Color(255, 0, 0, 255);
			else
				color = sf::Color(0, 255, 0, 255);

			rend::drawPoint(m_texture, point, color, width, height);
		}

		auto colors = lookUpTileMap.GetColorsInUse();
		std::cout << "Colors in use: " << colors << "\n";
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

	TileMap LookupMap::GenerateTileMapFromMask(const LookupFeatures& data, const NoiseData& noiseData, float borderThick, const MapMask* mapMask, TileType type, const char* name)
	{
		const auto width = Width();
		const auto height = Height();
		const auto buffer = mapMask->GetMaskBuffer();

		Mask mask(width, height, buffer);
		auto pointsContr = geomt::generatePointsConstrained<double>(data.tiles, data.seed, true, mask);
		mygal::Diagram<double>  diagram = std::move(geomt::generateDiagram(pointsContr));
		geomt::lloydRelaxation(diagram, data.lloyd, mask);
		

		TileMap maskTileMap(width, height);
		rasterizer::RasterizeDiagramToTileMap(diagram, width, height, maskTileMap, noiseData, borderThick);
		auto& tileMap = maskTileMap.GetTilesRef();

		maskTileMap.MarkTilesNotInMaskAsVisited(mask, type);

		std::vector<mygal::Vector2<double>> centroids;
		centroids.reserve(diagram.getSites().size());
		for (auto& site : diagram.getSites())
		{
			centroids.emplace_back(site.point);
		}
		maskTileMap.FloodFillTileMap(centroids);

		auto colorsNum = maskTileMap.GetColorsInUse();
		std::cout << "Colors in use in " << mapMask->Name() << " before second fill: " << colorsNum << "\n";
		std::unordered_set<Utils::Color> colors = maskTileMap.GetColors();

		maskTileMap.FloodFillMissingTiles(100);

		colorsNum = maskTileMap.GetColorsInUse();
		std::cout << "Colors in use in " << mapMask->Name() << " after second fill: " << colorsNum << "\n";

		//for (int y = 0; y < height; y++)
		//{
		//	for (int x = 0; x < width; x++)
		//	{
		//		auto index = y * width + x;
		//		if (tileMap[index].isBorder)
		//		{
		//			//auto color = FindClosestTileOfSameType(tileMap, x, y, width, height);
		//			auto tile = maskTileMap.GetTile(tileMap[index].centroid.x, tileMap[index].centroid.y);

		//			if(tile.color != Utils::Color(0, 0, 0, 0))
		//				tileMap[index].color = tile.color;
		//				
		//		}
		//	}
		//}


		return maskTileMap;
	}


	Utils::Color LookupMap::FindClosestTileOfSameType(const std::vector<Tile>& tileMap, int x, int y, unsigned width, unsigned height) const
	{
		std::unordered_set<mygal::Vector2<int>> tilesVisited;
		const  int startIdx = y * width + x;
		auto targetType = tileMap[startIdx].type;
		std::queue<std::pair<int, int>> queue;

		// Initialize BFS queue with the starting position
		queue.push({ x, y });

		// Directions array for exploring neighboring tiles: right, left, down, up

		while (!queue.empty())
		{
			auto [cx, cy] = queue.front();
			queue.pop();
			if (cx < 0 || cx >= width || cy < 0 || cy >= height) continue;
			if (tilesVisited.contains(mygal::Vector2<int>(cx, cy))) continue;

			const Tile& tile = tileMap[cy * width + cx];

			if (tile.type == targetType && (tile.visited) && (!tile.isBorder))
				return tile.color;
			else
				tilesVisited.insert(mygal::Vector2<int>(cx, cy));

			queue.push({ cx + 1, cy });
			queue.push({ cx - 1, cy });
			queue.push({ cx, cy + 1 });
			queue.push({ cx, cy - 1 });
			
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