#include <nexilis/ci.hh>
#include <cstdlib>

namespace nexilis
{

bool isRunningInCI()
{
    const char* ci_vars[]
    {
        "GITHUB_ACTIONS", // GitHub Actions
        "GITLAB_CI",      // GitLab CI
        nullptr
    };

    for (const char** var = ci_vars; *var; var++)
    {
        if (std::getenv(*var))
        {
            return true;
        }
    }
    return false;
}

}
