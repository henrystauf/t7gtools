// vdx.cpp

#include <vector>
#include <fstream>
#include <iostream>

#include "vdx.h"
#include "lzss.h"
#include "bitmap.h"

/*
===============================================================================
Function Name: parseVDXFile

Description:
    - Parses the VDX data and returns a VDXFile object containing the parsed data.

Parameters:
    - filename: The filename for the VDX data.
    - buffer: A vector containing the VDX data to be parsed.

Return:
    - A VDXFile object containing the parsed VDX data.

Notes:
    - None.
===============================================================================
*/
VDXFile parseVDXFile(const std::string &filename, const std::vector<uint8_t> &buffer)
{
    VDXFile vdxFile;
    vdxFile.filename = filename;

    vdxFile.identifier = buffer[0] | (buffer[1] << 8);
    std::copy(buffer.begin() + 2, buffer.begin() + 8, vdxFile.unknown.begin());

    size_t offset = 8;

    while (offset < buffer.size())
    {
        VDXChunk chunk;
        chunk.chunkType = buffer[offset];
        chunk.unknown = buffer[offset + 1];
        chunk.dataSize = buffer[offset + 2] | (buffer[offset + 3] << 8) | (buffer[offset + 4] << 16) | (buffer[offset + 5] << 24);
        chunk.lengthMask = buffer[offset + 6];
        chunk.lengthBits = buffer[offset + 7];
        offset += 8;
        chunk.data.assign(buffer.begin() + offset, buffer.begin() + offset + chunk.dataSize);
        offset += chunk.dataSize;

        vdxFile.chunks.push_back(chunk);
    }

    return vdxFile;
}

/*
===============================================================================
Function Name: parseVDXChunks

Description:
    - TBD

Parameters:
    - vdxFile: Fully populated VDXFile object.

Return:
    - TBD

Notes:
    - None.
===============================================================================
*/
void parseVDXChunks(VDXFile &vdxFile)
{
    for (VDXChunk &chunk : vdxFile.chunks)
    {
        std::vector<uint8_t> decompressedData;

        switch (chunk.chunkType)
        {
        case 0x00:
            // Handle chunk type 0x00
            break;
        case 0x20:
            // Handle chunk type 0x20
            decompressedData = lzssDecompress(chunk.data, chunk.lengthMask, chunk.lengthBits);
            // chunk.data = processType20Chunk(decompressedData);
            break;
        case 0x80:
            // Handle chunk type 0x80
            break;
        default:
            // Handle unknown chunk types
            break;
        }

        if (chunk.chunkType == 0x20)
        {
            std::string compressedFilename = vdxFile.filename + "-compressed.bin";
            std::ofstream compressedFile(compressedFilename, std::ios::binary);
            compressedFile.write(reinterpret_cast<const char *>(chunk.data.data()), chunk.data.size());

            std::string decompressedFilename = vdxFile.filename + "-decompressed.bin";
            std::ofstream decompressedFile(decompressedFilename, std::ios::binary);
            decompressedFile.write(reinterpret_cast<const char *>(decompressedData.data()), decompressedData.size());
        }
    }
}

/*
===============================================================================
Function Name: writeVDXFile

Description:
    - TBD

Parameters:
    - vdxFile: Always an std::vector of VDXFile type, but sometimes only a single
               VDXFile object is passed in.

Notes:
    - Used by the -x command-line switch as an Extraction Tool-set, not related
    to Game Engine functionality.
===============================================================================
*/
void writeVDXFile(const VDXFile &vdxFile, const std::string &outputDir)
{
    std::string vdxFileName = outputDir + "/" + vdxFile.filename;
    std::cout << "filename: " << vdxFileName << std::endl;

    std::ofstream vdxFileOut(vdxFileName, std::ios::binary);
    vdxFileOut.write(reinterpret_cast<const char *>(&vdxFile.identifier), sizeof(vdxFile.identifier));
    vdxFileOut.write(reinterpret_cast<const char *>(vdxFile.unknown.data()), 6);

    for (const auto &chunk : vdxFile.chunks)
    {
        // Write chunk header
        vdxFileOut.write(reinterpret_cast<const char *>(&chunk.chunkType), sizeof(chunk.chunkType));
        vdxFileOut.write(reinterpret_cast<const char *>(&chunk.unknown), sizeof(chunk.unknown));
        vdxFileOut.write(reinterpret_cast<const char *>(&chunk.dataSize), sizeof(chunk.dataSize));
        vdxFileOut.write(reinterpret_cast<const char *>(&chunk.lengthMask), sizeof(chunk.lengthMask));
        vdxFileOut.write(reinterpret_cast<const char *>(&chunk.lengthBits), sizeof(chunk.lengthBits));

        // Write chunk data
        vdxFileOut.write(reinterpret_cast<const char *>(chunk.data.data()), chunk.data.size());
    }

    vdxFileOut.close();
}