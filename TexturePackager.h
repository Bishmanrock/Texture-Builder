#ifndef TEXTURE_PACKAGER_H
#define TEXTURE_PACKAGER_H

#include <string>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct TextureMetadata
{
    char name[64]; // Fixed size of 64
    int width; // Width of image
    int height; // Height of image
    int nrChannels; // Number of color channels
    int32_t offset; // Offset in the file
    int32_t size; // Size in bytes
};
#pragma pack(pop)

bool readTextureFile(const std::string& filePath, std::vector<char>& data, TextureMetadata& metadata);
void packageTextures(const std::vector<std::string>& textureFiles, const std::string& outputFileName);

#endif