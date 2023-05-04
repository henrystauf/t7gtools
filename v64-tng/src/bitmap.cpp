// bitmap.cpp

#include <vector>
#include <cstdint>
#include <cstdio>
#include <stdexcept>

#include <png.h>

void savePNG(const std::string &filename, const std::vector<uint8_t> &imageData, int width, int height)
{
    FILE *fp = fopen(filename.c_str(), "wb");
    if (!fp)
    {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr)
    {
        fclose(fp);
        throw std::runtime_error("Failed to create PNG write struct");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fclose(fp);
        png_destroy_write_struct(&png_ptr, nullptr);
        throw std::runtime_error("Failed to create PNG info struct");
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        throw std::runtime_error("Error during PNG initialization");
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    std::vector<png_bytep> row_pointers(height);
    for (int y = 0; y < height; ++y)
    {
        row_pointers[y] = const_cast<uint8_t *>(&imageData[y * width * 3]);
    }

    png_write_image(png_ptr, row_pointers.data());

    png_write_end(png_ptr, nullptr);

    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

std::vector<uint8_t> processType20Chunk(const std::vector<uint8_t> &chunkData)
{
    // Parse the header
    uint16_t numXTiles = readLittleEndian<uint16_t>(decompressedData.data());
    uint16_t numYTiles = readLittleEndian<uint16_t>(decompressedData.data() + 2);
    uint16_t colourDepth = readLittleEndian<uint16_t>(decompressedData.data() + 4);

    const int width = numXTiles * 4;
    const int height = numYTiles * 4;
    const int numPixels = width * height;

    std::vector<RGBColor> palette;
    const uint8_t *paletteData = decompressedData.data() + 6;
    for (int i = 0; i < (1 << colourDepth); ++i)
    {
        palette.push_back({paletteData[i * 3], paletteData[i * 3 + 1], paletteData[i * 3 + 2]});
    }

    const uint8_t *imageData = paletteData + (1 << colourDepth) * 3;

    // Process the decompressed image data according to the Type 0x20 chunk specifications
    std::vector<uint8_t> outputImageData(numPixels * 3);
    for (int tileY = 0; tileY < numYTiles; ++tileY)
    {
        for (int tileX = 0; tileX < numXTiles; ++tileX)
        {
            uint8_t colour1 = *imageData++;
            uint8_t colour0 = *imageData++;
            uint16_t colourMap = readLittleEndian<uint16_t>(imageData);
            imageData += 2;

            for (int y = 0; y < 4; ++y)
            {
                for (int x = 0; x < 4; ++x)
                {
                    int globalX = tileX * 4 + x;
                    int globalY = tileY * 4 + y;
                    int pixelIndex = globalY * width + globalX;

                    uint16_t mask = 1 << (x + y * 4);
                    uint8_t colourIndex = (colourMap & mask) ? colour1 : colour0;

                    // Use colourIndex to get the actual RGB value from the palette and set the pixel value in the output image
                    RGBColor pixelColor = palette[colourIndex];
                    outputImageData[pixelIndex * 3] = pixelColor.r;
                    outputImageData[pixelIndex * 3 + 1] = pixelColor.g;
                    outputImageData[pixelIndex * 3 + 2] = pixelColor.b;
                }
            }
        }
    }

    return outputImageData;
}