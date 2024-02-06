#include "TexturePackager.h"
#include <fstream>
#include <iostream>
#include <cstring> // Include for memcpy
#include <algorithm> // For std::max and std::all_of

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Function to check if a string contains only ASCII characters
bool isAscii(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), [](unsigned char c) { return c <= 127; });
}


bool readTextureFile(const std::string& filePath, std::vector<char>& data, TextureMetadata& metadata)
{
    int width, height, channels;

    unsigned char* imageData = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);    
    
    if (!imageData)
    {
        std::cerr << "Failed to load image: " << filePath << std::endl;
        return false; // Failed to load image
    }

    metadata.width = width;
    metadata.height = height;
    metadata.nrChannels = channels; // Set the number of channels
    metadata.size = width * height * 4; // Assuming 4 bytes per pixel (RGBA)

    // Resize the data vector and copy the image data into it
    data.resize(metadata.size);
    memcpy(data.data(), imageData, metadata.size);

    stbi_image_free(imageData);
    return true;
}

// Removes the file extention, as this is not required once added to the /bin
std::string removeExtension(const std::string& fileName)
{
    size_t lastdot = fileName.find_last_of(".");
    if (lastdot == std::string::npos) return fileName;
    return fileName.substr(0, lastdot);
}

// Extracts the file name without the directory path and file extension
std::string getFileName(const std::string& filePath)
{
    // Find the last occurrence of both slash and backslash
    size_t lastSlashPos = filePath.find_last_of("/\\");
    if (lastSlashPos == std::string::npos)
    {
        // No slashes found, assume filePath is already just a file name
        return removeExtension(filePath);
    }
    std::string fileNameWithExtension = filePath.substr(lastSlashPos + 1);

    // Now remove the file extension
    return removeExtension(fileNameWithExtension);
}

// Packages the textures up into textures.bin
void packageTextures(const std::vector<std::string>& textureFilePaths, const std::string& outputFileName)
{
    std::ofstream outputFile(outputFileName, std::ios::binary);
    std::ofstream logFile("export_log.txt", std::ios::out);

    if (!outputFile.is_open() || !logFile.is_open())
    {
        std::cerr << "Error opening files" << std::endl;
        return;
    }

    std::vector<TextureMetadata> metadataList;

    long currentOffset = 0;

    for (const auto& filePath : textureFilePaths)
    {
        TextureMetadata metadata;
        std::vector<char> textureData;

        std::string textureName = getFileName(filePath);

        if (!isAscii(textureName)) // Check if the texture name is ASCII
        {
            std::cerr << "Texture name contains non-ASCII characters: " << textureName << std::endl;
            // Optionally, you can decide to skip this texture or handle the error differently
            continue; // Skip this texture
        }


        // Ensure the name fits in the fixed-size array
        memset(metadata.name, 0, sizeof(metadata.name)); // Initialize all characters to '\0', so that any unused in the char[64] are only 0's
        strncpy(metadata.name, textureName.c_str(), sizeof(metadata.name) - 1); // Copy name, ensure name fits in the fixed size array

        if (readTextureFile(filePath, textureData, metadata))
        {
            metadata.offset = currentOffset;
            outputFile.write(textureData.data(), textureData.size());
            currentOffset += metadata.size;

            metadataList.push_back(metadata); // Store metadata for later

                        // Log the details of the texture
            logFile << "Processed texture: " << metadata.name
                    << ", Width: " << metadata.width
                    << ", Height: " << metadata.height
                    << ", Size: " << metadata.size << " bytes"
                    << ", Offset: " << metadata.offset << std::endl;

        }
        else
        {
            logFile << "Failed to process texture: " << filePath << std::endl;
        }
    }

    // Write metadata at the end of the file
    for (const auto& md : metadataList)
    {
        outputFile.write(reinterpret_cast<const char*>(&md), sizeof(TextureMetadata));
    }



    // Write the number of metadata entries
    int metadataCount = static_cast<int>(textureFilePaths.size());
    outputFile.write(reinterpret_cast<char*>(&metadataCount), sizeof(metadataCount));


    outputFile.close();
    logFile.close();
}
