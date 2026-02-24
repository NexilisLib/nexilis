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

project("nexilis-premake")
kind("SharedLib")
language("C++")
targetdir("bin/%{cfg.buildcfg}")
objdir("obj/%{cfg.buildcfg}")

includedirs({
	"include",

	"third-party/boost/libs/static_assert/include",
	"third-party/boost/libs/intrusive/include",
	"third-party/boost/libs/endian/include",
	"third-party/boost/libs/predef/include",
	"third-party/boost/libs/mp11/include",
	"third-party/boost/libs/winapi/include",
	"third-party/boost/libs/core/include",
	"third-party/boost/libs/move/include",
	"third-party/boost/libs/container_hash/include",
	"third-party/boost/libs/describe/include",
	"third-party/boost/libs/container/include",
	"third-party/boost/libs/variant/include",
	"third-party/boost/libs/variant2/include",
	"third-party/boost/libs/throw_exception/include",
	"third-party/boost/libs/align/include",
	"third-party/boost/libs/config/include",
	"third-party/boost/libs/assert/include",
	"third-party/boost/libs/system/include",
	"third-party/boost/libs/json/include",
})

files({
	"src/nexilis/**.cc",
	"src/nexilisc/**.c",

	"include/nexilis/**.hh",
	"include/nexilisc/**.h",
})

-- Archived protocols
removefiles({
	"include/nexilis/archived_protocols/**.hh",
	"src/nexilis/archived_protocols/**.cc",
})

links({
	-- If using system boost
	"boost_system",
	"boost_json",
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
