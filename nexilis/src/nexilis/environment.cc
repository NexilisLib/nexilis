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

#include <cstdlib>
#include <nexilis/environment.hh>

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
