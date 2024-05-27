#pragma once
#include "basePanel.h"
#include <vector>
#include <SFML/Graphics/RenderTexture.hpp>
namespace MapGeneratorTool
{
namespace ui
{
class TexturePanel : public BasePanel
{
public:
	TexturePanel(const char* name) : BasePanel(name) {};
	void RenderPanel() const override;

	std::vector<const sf::RenderTexture*> texturesStack;
private:

};

}
}