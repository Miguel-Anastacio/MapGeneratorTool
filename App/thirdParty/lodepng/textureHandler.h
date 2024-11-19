#include <vector>
#include "lodepng.h"
#include <iostream>
namespace MapGeneratorTool
{
    namespace textureHandler
    {
        // Decode from disk to raw pixels with a single function call
        [[nodiscard]] std::vector<unsigned char> decodeOneStep(const char *filename);
        [[nodiscard]] std::vector<unsigned char> decodeOneStep(const char *filename, unsigned& width, unsigned& height);
        void encodeOneStep(const char *filename, const std::vector<unsigned char>& image, unsigned width, unsigned height);
    }
}