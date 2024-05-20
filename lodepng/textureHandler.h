#include <vector>
#include "lodepng.h"
#include <iostream>
namespace MapGeneratorTool
{
    namespace textureHandler
    {
        // Decode from disk to raw pixels with a single function call
        void decodeOneStep(const char *filename);
        void encodeOneStep(const char *filename, std::vector<unsigned char>& image, unsigned width, unsigned height);
    }
}