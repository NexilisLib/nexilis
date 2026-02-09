#ifndef NEXILIS_ASSETS_HH
#define NEXILIS_ASSETS_HH

#include <string>

namespace nexilis
{

class Assets
{
public:
    static bool assetsCreated();
    static const std::string& setAssetPath(const std::string& asset_path);
    static const std::string& getAssetPath();

private:
    static std::string determineDefaultPath();

private:
    static std::string m_assetPath;
    static std::string m_defaultPath;
};

} // namespace nexilis

#endif
