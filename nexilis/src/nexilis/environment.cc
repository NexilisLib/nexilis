#include <cstdlib>
#include <nexilis/environment.hh>

namespace nexilis
{

static bool isEnvDef(const char* var)
{
#if defined(_MSC_VER)
    // Use _dupenv_s for MSVC to avoid C4996 warning
    char* value = nullptr;
    size_t len = 0;
    errno_t err = _dupenv_s(&value, &len, var);
    bool defined = (err == 0 && value != nullptr);
    if (value)
    {
        free(value);
    }
    return defined;
#endif
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

    if (is_ci)
        return EnvironmentType::ci;
    if (is_nix)
        return EnvironmentType::nixos;
    return EnvironmentType::local;
}

std::string envToString(EnvironmentType type)
{
    switch (type)
    {
        case EnvironmentType::ci:
            return "ci";
        case EnvironmentType::nixos:
            return "nixos";
        case EnvironmentType::local:
            return "local";
    }
    return "";
}

} // namespace nexilis
