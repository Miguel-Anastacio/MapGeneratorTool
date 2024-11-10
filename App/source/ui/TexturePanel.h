#pragma once
#include "basePanel.h"
#include <vector>
#include <SFML/Graphics/RenderTexture.hpp>
namespace MapGeneratorTool
{
namespace ui
{
	static int mainImage = 0;
class TexturePanel : public BasePanel
{
public:
	TexturePanel(const char* name) : BasePanel(name) {};
	void RenderPanel() const override;

	std::vector<const sf::RenderTexture*> texturesStack;
private:
	void HandleMouseInput(float& zoom, ImVec2& panOffset, const ImVec2& imageSize) const;
};

}
}

inline ImVec2 clamp(const ImVec2& vector, const ImVec2& min, const ImVec2& max)
{
	return ImVec2(std::clamp(vector.x, min.x, max.x), std::clamp(vector.y, min.y, max.y));
}
//
//ImVec2 operator+=(ImVec2& lhs, const ImVec2& rhs)
//{
//	lhs.x += rhs.x;
//	lhs.y += rhs.y;
//	return lhs;
//}
////
//ImVec2 operator*(const ImVec2& lhs, float scalar)
//{
//	return ImVec2(lhs.x * scalar, lhs.y * scalar);
//}
////
//////ImVec2 operator*=(ImVec2& lhs, const ImVec2& rhs)
//////{
//////	lhs.x *= rhs.x;
//////	lhs.y *= rhs.y;
//////	return lhs;
//////}