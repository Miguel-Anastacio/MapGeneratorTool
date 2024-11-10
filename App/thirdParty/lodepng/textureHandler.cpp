#include "textureHandler.h"
namespace MapGeneratorTool
{
    namespace textureHandler
    {
        // Decode from disk to raw pixels with a single function call
        std::vector<unsigned char> decodeOneStep(const char *filename, unsigned& width, unsigned& height)
        {
            std::vector<unsigned char> image; // the raw pixels
            unsigned w, h;

            // decode
            unsigned error = lodepng::decode(image, w, h, filename);

            // if there's an error, display it
            if (error)
                std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

            width = w;
            height = h;
            // the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
            return image;
        }

        std::vector<unsigned char> decodeOneStep(const char* filename)
        {
            std::vector<unsigned char> image; // the raw pixels
            unsigned width, height;

            // decode
            unsigned error = lodepng::decode(image, width, height, filename);

            // if there's an error, display it
            if (error)
                std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

            // the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
            return image;
        }


        //Example 1
        //Encode from raw pixels to disk with a single function call
        //The image argument has width * height RGBA pixels or width * height * 4 bytes
        void encodeOneStep(const char* filename, const std::vector<unsigned char>& image, unsigned width, unsigned height) {
            //Encode the image
            unsigned error = lodepng::encode(filename, image, width, height);

            //if there's an error, display it
            if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        }


    }
}