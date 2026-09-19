/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

#include <nexilis/assets.hh>
#include <nexilis/logger/log.hh>

namespace nexilis
{

std::string Assets::m_assetPath;
std::string Assets::m_defaultPath;

bool Assets::assetsCreated()
{
    if (!m_assetPath.empty())
    {
        return true;
    }
    Log::warning("Asset path not found!");
    return false;
}

const std::string& Assets::getAssetPath()
{
    if (!m_assetPath.empty())
    {
        return m_assetPath;
    }
    else
    {
        Log::info("Asset path not defined, using default path: ", m_defaultPath);
        return m_defaultPath;
    }
}

const std::string& Assets::setAssetPath(const std::string& asset_path)
{
    if (!asset_path.empty() && asset_path[0] == '~')
    {
        const char* home = std::getenv("HOME");
        if (home)
        {
            m_assetPath = home + asset_path.substr(1);
        }
        else
        {
            m_assetPath = asset_path;
        }
    }
    else
    {
        m_assetPath = asset_path;
    }
    return m_assetPath;
}

std::string Assets::determineDefaultPath()
{
    // Get the home directory
    const char* homeDir = std::getenv("HOME");
    if (!homeDir)
    {
        // Fallback for Windows
        homeDir = std::getenv("USERPROFILE");
    }

    // Build the path
    std::filesystem::path path(homeDir);
    path /= ".local";
    path /= "share";
    path /= "nexilis";

    // Create directory if it doesn't exist.
    std::filesystem::create_directories(path);

    // Return as string with trailing slash
    return path.string() + "/";
}

} // namespace nexilis
