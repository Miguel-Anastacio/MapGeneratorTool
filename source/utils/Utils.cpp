#include "MyGAL/Vector2.h"
#include "Utils.h"
#include <unordered_set>
namespace MapGeneratorTool
{
namespace Utils
{
Color GetRandomColorNotInSet(const std::unordered_set<Utils::Color>& set)
{
	Utils::Color color;
	do {
		color.RandColor();
	} while (set.contains(color) || color == Utils::Color(255, 255, 255, 255));

	return color;
}
}
}
