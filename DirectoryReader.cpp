// Script to scan for image files in the existing folder

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

// Helper function to convert string to lowercase
std::string toLower(const std::string& str)
{
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

bool endsWith(const std::string& fullString, const std::string& ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

// Check if the file has a texture extension, currently turned off to allow all files
bool isTextureFile(const std::string& fileName)
{
    std::vector<std::string> textureExtensions = {".png", ".jpg", ".jpeg"};

    std::string lowerFileName = toLower(fileName);

    for (const auto& ext : textureExtensions)
    {
        if (endsWith(lowerFileName, ext))
        {
            return true;
        }
    }

    return false;
}

// Include platform-specific headers
#if defined(_WIN32)
    #include <windows.h>
    #include <codecvt>
    #include <locale>
#elif defined(__linux__) || defined(__unix__)
    #include <dirent.h>
    #include <sys/types.h>
#endif

std::vector<std::string> getTextureFiles(const std::string& directoryPath)
{
    std::vector<std::string> textureFiles;

    // Windows-specific directory traversal
    #if defined(_WIN32)
    WIN32_FIND_DATAW findFileData;

    // Convert directoryPath to wide string
    std::wstring wideDirectoryPath = std::wstring(directoryPath.begin(), directoryPath.end());

    HANDLE hFind = FindFirstFileW((wideDirectoryPath + L"\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // Convert from WCHAR (wide char) to std::string (narrow char)
            std::wstring wideFileName = findFileData.cFileName;
            std::string fileName(wideFileName.begin(), wideFileName.end());

            if (fileName == "." || fileName == "..")
                {
                    continue; // Skip the current and parent directory entries
                }

            //if (isTextureFile(fileName))
            //{
                // Check if the file is a texture
                textureFiles.push_back(directoryPath + "/" + fileName);
            //}
        }
        while (FindNextFileW(hFind, &findFileData) != 0);

        FindClose(hFind);
    }

    // Linux/Unix-specific directory traversal
    #elif defined(__linux__) || defined(__unix__)
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(directoryPath.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string fileName = ent->d_name;

            if (fileName == "." || fileName == "..")
            {
                continue; // Skip the current and parent directory entries
            }

            if (isTextureFile(fileName))
            { 
                fileName = removeExtension(fileName); // Remove extension before adding to the list
                textureFiles.push_back(directoryPath + "/" + fileName);
            }

            if (isTextureFile(fileName))
            { 
                // Check if the file is a texture
                textureFiles.push_back(directoryPath + "/" + fileName);
            }

        }
        closedir(dir);
    }
    #endif

    return textureFiles;
}