--     Copyright (C) 2026 Valtteri Viirret
--     This file is part of the Nexilis Project.
--
--     This file is free software: you can redistribute it and/or modify
--     it under the terms of the GNU Lesser General Public License as
--     published by the Free Software Foundation, either version 3 of the
--     License, or (at your option) any later version.
--
--     This file is distributed in the hope that it will be useful,
--     but WITHOUT ANY WARRANTY; without even the implied warranty of
--     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--     GNU Lesser General Public License for more details.
--
--     You should have received a copy of the GNU Lesser General Public License
--     along with this file.  If not, see <https://gnu.org>.

workspace("nexilis")
configurations({ "Debug", "Release" })
architecture("x64")
cppdialect("C++17")
staticruntime("Off")
systemversion("latest")

filter("configurations:Debug")
defines({ "DEBUG" })
symbols("On")
filter("configurations:Release")
defines({ "NDEBUG" })
optimize("On")

filter("system:linux")
defines({ "LINUX", "POSIX" })
buildoptions({ "-Wall", "-Wextra", "-Wpedantic", "-fPIC" })
linkoptions({
	"-pthread",
	"-static-libstdc++",
	"-static-libgcc",
})

-- All boost submodule headers. Keeps every translation unit on the exact
-- same boost version, no matter which system boost is installed.
local boost_includes = os.matchdirs("third-party/boost/libs/*/include")
-- Modules that live nested inside another module directory.
table.insert(boost_includes, "third-party/boost/libs/numeric/conversion/include")

-- Boost libraries with compiled sources (mirrors BOOST_LIBS_TO_BUILD
-- in CMakeLists.txt).
project("nexilis-boost")
kind("StaticLib")
language("C++")
warnings("Off")
pic("On")
targetdir("bin/%{cfg.buildcfg}")
objdir("obj/nexilis-boost/%{cfg.buildcfg}")
includedirs(boost_includes)
files({
	"third-party/boost/libs/json/src/**.cpp",
	"third-party/boost/libs/container/src/**.cpp",
	"third-party/boost/libs/system/src/**.cpp",
})

project("nexilis-premake")
kind("SharedLib")
language("C++")
targetdir("bin/%{cfg.buildcfg}")
objdir("obj/%{cfg.buildcfg}")

includedirs({
	"include",
	boost_includes,
})

files({
	"src/nexilis/**.cc",
	"src/nexilisc/**.c",

	"include/nexilis/**.hh",
	"include/nexilisc/**.h",
})

links({
	"nexilis-boost",
	"crypto",
	"ssl",
})

-- Post-build message
postbuildcommands({
	"echo 'Built libnexilis.so in %{cfg.targetdir}'",
})

filter("configurations:Debug")
targetsuffix("_d")
defines({ "DEBUG" })
symbols("On")
filter("configurations:Release")
targetsuffix("_r")
defines({ "NDEBUG" })
optimize("On")
