#include <nexilis/environment.hh>
#include <cstdlib>

namespace nexilis
{

static bool isEnvDef(const char* var)
{
    return std::getenv(var) != nullptr;
}

EnvironmentType detectRuntimeType()
{
    const char* ci_vars[]{
            "GITHUB_ACTIONS",
            "GITLAB_CI",
            "CI",
            "TRAVIS",
            "CIRCLECI",
            nullptr};

    // Runtime detection first
    bool is_ci = false;
    bool is_nix = false;

    for (const char** var = ci_vars; *var; var++)
    {
        if (isEnvDef(*var))
        {
            is_ci = true;
            break;
        }
    }

    if (isEnvDef("NIX_BUILD_CORES"))
    {
        is_nix = true;
    }

#if defined(NEXILIS_IS_CI)
 #if NEXILIS_IS_CI == 1
    static_assert(NEXILIS_IS_CI == is_ci);
 #endif
#endif

#if defined(NEXILIS_IS_NIX)
#if NEXILIS_IS_NIX == 1
    static_assert(NEXILIS_IS_NIX == is_nix);
#endif
#endif

    if (is_ci) return EnvironmentType::ci;
    if (is_nix) return EnvironmentType::nixos;
    return EnvironmentType::local;
}

std::string envToString(EnvironmentType type)
{
    switch (type)
    {
        case EnvironmentType::ci: return "ci";
        case EnvironmentType::nixos: return "nixos";
        case EnvironmentType::local: return "local";
    }
    return "";
}

} // namespace nexilis
