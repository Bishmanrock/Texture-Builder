#include "TexturePackager.h"
#include <iostream>
#include <iostream>
#include <vector>
#include <string>

#include "DirectoryReader.h"


int main()
{
    std::string directoryPath = "./textures"; // Where to get the files from

    std::vector<std::string> textures = getTextureFiles(directoryPath);

    if (!textures.empty())
    {
        packageTextures(textures, "textures.bin");

        std::cout << "Textures packaged successfully." << std::endl;
    }
    else
    {
        std::cout << "No texture files found." << std::endl;
    }

    std::cout << "Press Enter to continue...";

    std::cin.get();
    
    return 0;
}