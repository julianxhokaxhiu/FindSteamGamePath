/*****************************************************************************/
// Copyright (C) 2024 Julian Xhokaxhiu                                       //
//                                                                           //
// This file is part of FindSteamGamePath                                    //
//                                                                           //
// FindSteamGamePath is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by      //
// the Free Software Foundation, either version 3 of the License             //
//                                                                           //
// FindSteamGamePath is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
// GNU General Public License for more details.                              //
/*****************************************************************************/

#include <iostream>
#include <filesystem>
#include <windows.h>

#include "vdf_parser.hpp"
#include "resource.h"
#include "utils.h"

namespace fs = std::filesystem;

// Function to find Steam installation path (Windows-specific)
std::string GetSteamPath()
{
    HKEY hKey;
    const char *regPath = "Software\\Valve\\Steam";
    const char *valueName = "SteamPath";
    char steamPath[512];
    DWORD bufferSize = sizeof(steamPath);

    if (RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueExA(hKey, valueName, nullptr, nullptr, (LPBYTE)steamPath, &bufferSize) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return std::string(steamPath);
        }
        RegCloseKey(hKey);
    }

    return "";
}

int main(int argc, char *argv[])
{
    // Retrieve the first argument (the entire command line is in lpCmdLine)
    std::string appId = argc > 1 ? argv[1] : "";

    if (!appId.empty())
    {
        std::string installPath;
        std::string steamPath = GetSteamPath();

        if (!steamPath.empty())
        {
            std::string libraryFoldersPath = steamPath + "/steamapps/libraryfolders.vdf";

            if (fs::exists(libraryFoldersPath))
            {
                std::ifstream libraryStream(libraryFoldersPath);
                if (!libraryStream.is_open())
                {
                    std::cerr << "Could not open libraryfolders.vdf file." << std::endl;
                    return 1;
                }

                try
                {
                    auto root = tyti::vdf::read(libraryStream);

                    // Traverse the library folders in VDF
                    for (const auto &section : root.childs)
                    {
                        // Retrieve the path for each library
                        std::string libraryPath = section.second->attribs["path"];

                        // Normalize path separators
                        std::replace(libraryPath.begin(), libraryPath.end(), '\\', '/');

                        // Check if the game manifest exists
                        std::string manifestPath = libraryPath + "/steamapps/appmanifest_" + appId + ".acf";
                        if (fs::exists(manifestPath))
                        {
                            std::ifstream manifestStream(manifestPath);
                            if (!manifestStream.is_open())
                            {
                                std::cerr << "Could not open " << manifestPath << " file. Corrupt installation?" << std::endl;
                                return 1;
                            }
                            auto manifestRoot = tyti::vdf::read(manifestStream);
                            installPath = libraryPath + "/steamapps/common/" + manifestRoot.attribs["name"];
                            break;
                        }
                    }

                    // Output the result
                    if (!installPath.empty())
                    {
                        std::cout << installPath << std::endl;
                    }
                    else
                    {
                        std::cerr << "Install path not found." << std::endl;
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error parsing VDF file: " << e.what() << std::endl;
                }
            }
            else
            {
                std::cerr << "libraryfolders.vdf file not found." << std::endl;
            }
        }
        else
        {
            std::cerr << "Steam installation not found." << std::endl;
        }
    }
    else
    {
        std::cerr << "No AppID given." << std::endl;
    }

    return 0;
}
