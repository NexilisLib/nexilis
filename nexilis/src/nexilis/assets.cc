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
