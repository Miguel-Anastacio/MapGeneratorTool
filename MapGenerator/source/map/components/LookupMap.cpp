#pragma once
#include <algorithm>
#include "MyGAL/Vector2.h"
// #include "Renderer.h"
#include "LookupMap.h"
#include "MapMask.h"
#include "utils/ComputeGeometry.h"
#include "data/Mask.h"
#include <queue>
#include "utils/Algo.h"
#include "data/Color.h"
#include "utils/VectorWrapper.h"
#include "Utilities/Timer.h"
#include "Utilities/Logger.h"
// #include "ThreadPool.h"
#include "FileIO/FileHandler.h"
namespace MapGenerator
{

	LookupMap::LookupMap(const char *name, unsigned width, unsigned height)
		: MapComponent(width, height, name)
	{
	}

	void LookupMap::RegenerateLookUp(const LookupMapData &data, MapMask *landMask, MapMask *oceanMask)
	{
		const auto width = Width();
		const auto height = Height();

		computeDiagramFromMask(data.land, landMask, m_landDiagram);
		computeDiagramFromMask(data.ocean, oceanMask, m_oceanDiagram);

		RegenerateBorders(data, landMask, oceanMask);

		// std::unordered_set <data::Color> colorsInUse;
		// auto landTileMap = GenerateTileMapFromMask(m_landDiagram, data.borderNoise, data.borderLine, landMask, TileType::LAND, colorsInUse, "landMaskLookUp.png");
		// auto oceanTileMap = GenerateTileMapFromMask(m_oceanDiagram, data.borderNoise, data.borderLine, oceanMask, TileType::WATER, colorsInUse,"oceanMaskLookUp.png");

		// thread::ThreadPool threadPool(1);
		// threadPool.submit(rend::drawTileMap, landTileMap, std::ref(landMask->Texture()), width, height);
		// threadPool.submit(rend::drawTileMap, oceanTileMap, std::ref(oceanMask->Texture()), width, height);
		// TileMap lookUpTileMap = TileMap::BlendTileMap(landTileMap, TileType::LAND, oceanTileMap, TileType::WATER);
		// threadPool.submit(rend::drawTileMap, lookUpTileMap, std::ref(m_texture), width, height);

		////Logger::LogObject(Logger::Type::INFO, "Colors in set ", colorsInUse.size());
		////Logger::LogObject(Logger::Type::INFO, "Colors in tileMap ", lookUpTileMap.GetColorsInUse());
		// Logger::LogObject(Logger::Type::INFO, "", lookUpTileMap);

		OutputLookupTable();
	}

	void LookupMap::RegenerateBorders(const LookupMapData &data, MapMask *landMask, MapMask *oceanMask)
	{
		const auto width = Width();
		const auto height = Height();
		std::unordered_set<data::Color> colorsInUse;
		auto landTileMap = GenerateTileMapFromMask(m_landDiagram, data.borderNoise, data.borderLine, landMask, TileType::LAND, colorsInUse, "landMaskLookUp.png");
		auto oceanTileMap = GenerateTileMapFromMask(m_oceanDiagram, data.borderNoise, data.borderLine, oceanMask, TileType::WATER, colorsInUse, "oceanMaskLookUp.png");

	/*	landMask->Texture().clear();
		oceanMask->Texture().clear();
		rend::drawTileMap(landTileMap, landMask->Texture(), width, height);
		rend::drawTileMap(oceanTileMap, oceanMask->Texture(), width, height);*/

		TileMap lookUpTileMap = TileMap::BlendTileMap(landTileMap, TileType::LAND, oceanTileMap, TileType::WATER);
		//m_texture.clear();
		//rend::drawTileMap(lookUpTileMap, m_texture, width, height);

		m_lookUpTileMap = std::make_unique<TileMap>(std::move(lookUpTileMap));

		Core::Logger::LogObject(Core::Logger::Type::INFO, "", *m_lookUpTileMap);
	}

	TileMap LookupMap::GenerateTileMapFromMask(const std::shared_ptr<Diagram> &diagram, const NoiseData &noiseData, float borderThick,
											   const MapMask *mapMask, TileType type,
											   std::unordered_set<data::Color> &colors,
											   const char *name)
	{
		const auto width = Width();
		const auto height = Height();

		assert(diagram != nullptr);

		TileMap maskTileMap(width, height);

		rasterizer::RasterizeDiagramToTileMap(*diagram, width, height, maskTileMap, noiseData, (double)borderThick);

		maskTileMap.MarkTilesNotInMaskAsVisited(mapMask->GetMask(), type);

		std::vector<mygal::Vector2<double>> centroids = geomt::GetCentroidsOfDiagram(*diagram);
		maskTileMap.FloodFillTileMap(centroids, colors);
		// auto colorsNum = maskTileMap.GetColorsInUse();
		// std::cout << "Colors in use in " << mapMask->Name() << " before second fill: " << colorsNum << "\n";

		maskTileMap.FloodFillMissingTiles(100);
		// colorsNum = maskTileMap.GetColorsInUse();
		// std::cout << "Colors in use in " << mapMask->Name() << " after second fill: " << colorsNum << "\n";

		maskTileMap.ColorInBorders(mapMask->GetMask());

		return maskTileMap;
	}

	void LookupMap::OutputLookupTable() const
	{
		/// print a json file whit an id associated with a colour
		nlohmann::ordered_json lookUpArray = nlohmann::ordered_json::array();
		nlohmann::ordered_json centroidArray = nlohmann::ordered_json::array();

		int i = 0;
		for (auto cell : m_lookUpTileMap->GetCellMap())
		{
			lookUpArray.push_back({{"Name", i}, {"Color", cell.second.ConvertToHex()}});
			nlohmann::json j = cell.first;
			centroidArray.push_back({{"Centroid", i}, {"Position", j}});
			i++;
		}
		Core::FileHandler::OutputJsonArrayToFIle("lookup.json", lookUpArray);
		Core::FileHandler::OutputJsonArrayToFIle("centroid.json", centroidArray);
	}

	void LookupMap::computeDiagramFromMask(const LookupFeatures &data, MapMask *mask, std::shared_ptr<Diagram> &diagram) const
	{
		const auto width = Width();
		const auto height = Height();

		Core::Timer timer;
		auto pointsContr = geomt::generatePointsConstrained<double>(data.tiles, data.seed, true, mask->GetMask());
		diagram = std::make_shared<Diagram>(geomt::generateDiagram(pointsContr));
		geomt::lloydRelaxation(*diagram.get(), data.lloyd, mask->GetMask());

		Core::Logger::TimeMsec("Generate Diagram:", timer.elapsedMilliseconds());
	}

}