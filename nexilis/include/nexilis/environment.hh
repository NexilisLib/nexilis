#ifndef NEXILIS_CI_HH
#define NEXILIS_CI_HH

#include <string>

namespace nexilis
{

enum class EnvironmentType
{
    ci,
    nixos,
    local
};

EnvironmentType detectRuntimeType();
std::string envToString(EnvironmentType type);

} // namespace nexilis

#endif
