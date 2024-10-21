#include <Renderer.h>
#include <TileMap.h>
namespace MapGeneratorTool
{
namespace rend
{
    void saveToFile(const sf::RenderTexture& texture, const char* filename)
    {
        texture.getTexture().copyToImage().saveToFile(filename);
    }

    void drawBuffer(const std::vector<sf::Uint8>& buffer, sf::RenderTexture& renderTexture, unsigned width, unsigned height)
    {
        //auto start = std::chrono::steady_clock::now();
        if (renderTexture.getSize() == sf::Vector2u(0, 0))
        {
            if (!renderTexture.create(width, height))
            {
                // error...
                std::cout << "Error creating texture to draw polygons to map" << "\n";
                return;
            }
        }
        // Create a texture
        sf::Texture texture;
        if (!texture.create(width, height)) {
            return; // Error handling
        }

        // Update the texture with the pixel buffer
        texture.update(buffer.data());

        renderTexture.clear();
        sf::Sprite sprite(texture);
        renderTexture.draw(sprite);
        renderTexture.display();
        //auto duration = std::chrono::steady_clock::now() - start;
        //std::cout << "draw buffer: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
    }

    void drawTileMap(const TileMap& tileMap, sf::RenderTexture& renderTexture, unsigned width, unsigned height)
    {
        drawBuffer(tileMap.ConvertTileMapToBuffer(), renderTexture, width, height);
    }

} // namespace rend
} // namespace MapGeneratorTool