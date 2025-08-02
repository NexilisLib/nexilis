#include <cstdlib>
#include <nexilis/ci.hh>

namespace nexilis
{

// TODO return "EnvironmentType"
bool isRunningInCI()
{
    const char* ci_vars[]{
            "GITHUB_ACTIONS", // GitHub Actions
            "GITLAB_CI",      // GitLab CI
            nullptr};

    for (const char** var = ci_vars; *var; var++)
    {
        if (std::getenv(*var))
        {
            return true;
        }
    }

#if defined(NEXILIS_IS_LOCAL)
    static_assert(NEXILIS_IS_LOCAL == 1);
#endif

#if defined(NEXILIS_IS_CI)
    static_assert(NEXILIS_IS_CI == 0);
#endif

    return false;
}

} // namespace nexilis
