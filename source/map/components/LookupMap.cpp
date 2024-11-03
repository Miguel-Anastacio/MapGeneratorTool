#pragma once
#include <algorithm>
#include "MyGAL/Vector2.h"
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
#include "VectorWrapper.h"
#include "Timer.h"
#include "Logger.h"
#include "ThreadPool.h"
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

		computeDiagramFromMask(data.land, landMask, m_landDiagram);
		computeDiagramFromMask(data.ocean, oceanMask, m_oceanDiagram);

		auto landTileMap = GenerateTileMapFromMask(m_landDiagram, data.borderNoise, data.borderLine, landMask, TileType::LAND, "landMaskLookUp.png");
		auto oceanTileMap = GenerateTileMapFromMask(m_oceanDiagram, data.borderNoise, data.borderLine, oceanMask, TileType::WATER, "oceanMaskLookUp.png");


		thread::ThreadPool threadPool(1);
		threadPool.submit(rend::drawTileMap, landTileMap, std::ref(landMask->Texture()), width, height);
		threadPool.submit(rend::drawTileMap, oceanTileMap, std::ref(oceanMask->Texture()), width, height);

		TileMap lookUpTileMap = TileMap::BlendTileMap(landTileMap, TileType::LAND, oceanTileMap, TileType::WATER);
		threadPool.submit(rend::drawTileMap, lookUpTileMap, std::ref(m_texture), width, height);

		auto colors = lookUpTileMap.GetColorsInUse();
		std::cout << "Colors in use: " << colors << "\n";
		//OutputLookupTable();

	}

	void LookupMap::RegenerateBorders(const LookupMapData& data, MapMask* landMask, MapMask* oceanMask)
	{
		const auto width = Width();
		const auto height = Height();
		m_colorsInUse.clear();
		auto landTileMap = GenerateTileMapFromMask(m_landDiagram, data.borderNoise, data.borderLine, landMask, TileType::LAND, "landMaskLookUp.png");
		auto oceanTileMap = GenerateTileMapFromMask(m_oceanDiagram, data.borderNoise, data.borderLine, oceanMask, TileType::WATER, "oceanMaskLookUp.png");

		landMask->Texture().clear();
		oceanMask->Texture().clear();
		rend::drawTileMap(landTileMap, landMask->Texture(), width, height);
		rend::drawTileMap(oceanTileMap, oceanMask->Texture(), width, height);

		TileMap lookUpTileMap = TileMap::BlendTileMap(landTileMap, TileType::LAND, oceanTileMap, TileType::WATER);
		m_texture.clear();
		rend::drawTileMap(lookUpTileMap, m_texture, width, height);

	}
	TileMap LookupMap::GenerateTileMapFromMask(const std::shared_ptr<Diagram>& diagram, const NoiseData& noiseData, float borderThick, const MapMask* mapMask, TileType type, const char* name)
	{
		const auto width = Width();
		const auto height = Height();

		assert(diagram != nullptr);

		TileMap maskTileMap(width, height);
		rasterizer::RasterizeDiagramToTileMap(*diagram, width, height, maskTileMap, noiseData, (double)borderThick);
		auto& tileMap = maskTileMap.GetTilesRef();

		maskTileMap.MarkTilesNotInMaskAsVisited(mapMask->GetMask(), type);

		std::vector<mygal::Vector2<double>> centroids;
		centroids.reserve(diagram->getSites().size());
		for (auto& site : diagram->getSites())
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

		Timer timer;
		for (unsigned y = 0; y < height; y++)
		{
			for (unsigned x = 0; x < width; x++)
			{
				auto index = y * width + x;
				if (tileMap[index].isBorder && mapMask->GetMask().isInMask(x, y))
				{
					Utils::Color color;
					if (maskTileMap.FindColorOfClosestTileOfSameType(x, y, width, color))
					{
						tileMap[index].color = color;
					}
				}
			}
		}
		Logger::TimeMsec("Time set border colors", timer.elapsedMilliseconds());
		return maskTileMap;
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

	void LookupMap::computeDiagramFromMask(const LookupFeatures& data, MapMask* mask, std::shared_ptr<Diagram>& diagram) const
	{
		const auto width = Width();
		const auto height = Height();

		Timer timer;
		auto pointsContr = geomt::generatePointsConstrained<double>(data.tiles, data.seed, true, mask->GetMask());
		diagram = std::make_shared<Diagram>(geomt::generateDiagram(pointsContr));
		geomt::lloydRelaxation(*diagram.get(), data.lloyd, mask->GetMask());

		Logger::TimeMsec("Generate Diagram:", timer.elapsedMilliseconds());
	}

}