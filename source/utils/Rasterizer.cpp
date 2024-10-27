#include <Rasterizer.h>
namespace MapGeneratorTool
{
namespace rasterizer
{
	void plotTile(int x, int y, unsigned width, unsigned height, std::vector<Tile>& tileMap)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			int index = y * width + x;
			tileMap[index].visited = true; // Mark the tile as visited
			tileMap[index].isBorder = true; // Mark the tile as visited
		}
	}

	void plotLineWithWidth(const mygal::Vector2<int>& start, const mygal::Vector2<int>& end, std::vector<Tile>& tileMap, unsigned width, unsigned height,
		FastNoiseLite& noise, float noiseScale, float wd)
	{
		int x0 = start.x;
		int y0 = start.y;

		int x1 = end.x;
		int y1 = end.y;

		int dx = abs(end.x - start.x), sx = start.x < end.x ? 1 : -1;
		int dy = abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
		int err = dx - dy, e2, x2, y2;                          /* error value e_xy */
		float ed = dx + dy == 0 ? 1 : std::sqrt((float)dx * dx + (float)dy * dy);

		std::vector<mygal::Vector2<int>> line;
		for (wd = (wd + 1) / 2; ; )
		{                                   /* pixel loop */
			line.emplace_back(x0, y0);
			e2 = err; x2 = x0;
			if (2 * e2 >= -dx)
			{                                           /* x step */
				for (e2 += dy, y2 = y0; e2 < ed * wd && (y1 != y2 || dx > dy); e2 += dx)
					line.emplace_back(x0, y2 += sy);
				if (x0 == x1) break;
				e2 = err; err -= dy; x0 += sx;
			}
			if (2 * e2 <= dy)
			{                                            /* y step */
				for (e2 = dx - e2; e2 < ed * wd && (x1 != x2 || dx < dy); e2 += dy)
					line.emplace_back(x2 += sx, y0);

				if (y0 == y1) break;
				err += dx; y0 += sy;
			}
		}

		geomt::DeformLine(line, noise, noiseScale);

		for (auto& point : line)
		{
			plotTile(point.x, point.y, width, height, tileMap); // Cast coordinates to integers and plot
		}
	}


} // namespace MapGeneratorTool
} // namespace rasterizer
