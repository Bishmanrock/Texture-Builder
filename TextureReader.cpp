// Script that scans and confirms the contents of a textures.bin

#include <iostream>
#include <fstream>
#include <vector>
#include "TexturePackager.h" // Assuming this is where TextureMetadata is defined

std::ofstream logFile; // Global log file stream

const int maxTextureWidth = 4096; // Maximum texture width allowed
const int maxTextureHeight = 4096; // Maximum texture height allowed
const int bytesPerPixel = 4; // 4 bytes per pixel for RGBA

// Calculate the maximum texture size with a 10% safety margin
const size_t maxTextureSize = static_cast<size_t>(maxTextureWidth * maxTextureHeight * bytesPerPixel * 1.1);

// Use maxTextureSize as the upper limit in your verification logic


std::vector<TextureMetadata> readMetadata(std::ifstream& file)
{
    std::vector<TextureMetadata> metadataList;

    // Move to the end of the file minus the size of the metadata count, to get the count of how many files are there
    file.seekg(-static_cast<int>(sizeof(int)), std::ios::end);
    int metadataCount;
    file.read(reinterpret_cast<char*>(&metadataCount), sizeof(metadataCount));


    // Move to the start of the metadata
    file.seekg(-static_cast<int>(sizeof(int) + (sizeof(TextureMetadata) * metadataCount)), std::ios::end);

        // Read all metadata entries
    for (int i = 0; i < metadataCount; ++i)
    {
        TextureMetadata metadata;
        file.read(reinterpret_cast<char*>(&metadata), sizeof(TextureMetadata));
        metadataList.push_back(metadata);
    }

    return metadataList;
}

bool verifyTextureData(std::ifstream& file, const TextureMetadata& metadata)
{
    const size_t maxTextureSize = static_cast<size_t>(maxTextureWidth * maxTextureHeight * bytesPerPixel * 1.1); // Calculate max file size

     // Check if metadata.size is reasonable
    if (metadata.size <= 0 || metadata.size > maxTextureSize)
    {
        logFile << "Invalid texture size for: " << metadata.name << std::endl;
        return false;
    }

    file.seekg(metadata.offset, std::ios::beg);
    std::vector<char> buffer(metadata.size);


    file.read(buffer.data(), metadata.size);
    return file.gcount() == metadata.size;
}

int main()
{
    logFile.open("TextureReader.log");

        if (!logFile.is_open())
        {
        std::cerr << "Failed to open log file." << std::endl;
        return 1;
    }

    std::ifstream inputFile("textures.bin", std::ios::binary);

    if (!inputFile.is_open())
    {
        logFile << "Failed to open textures.bin" << std::endl;
        return 1;
    }

    auto metadataList = readMetadata(inputFile);

    for (const auto& metadata : metadataList)
    {
        if (verifyTextureData(inputFile, metadata))
        {
                // Log the entire texture name at once, not character by character
                logFile << " " << std::endl;
                logFile << "TEXTURE DETAILS" << std::endl;
                logFile << "Verified Name: " << metadata.name << std::endl;
                logFile << "Verified Width: " << metadata.width << std::endl;
                logFile << "Verified Height: " << metadata.height << std::endl;
                logFile << "Verified Channels: " << metadata.nrChannels << std::endl;
                logFile << "Verified Size: " << metadata.size << std::endl;
                logFile << "Verified Offset: " << metadata.offset << std::endl;
        }
        else
        {
        logFile << "Failed to verify texture: " << metadata.name << std::endl;
        }
    }

    // Close the log file
    logFile.close();

        // Keep the console window open (optional, for debugging)
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}
