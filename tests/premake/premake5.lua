workspace("NexilisTest")
configurations({ "Debug" })
architecture("x64")

project("premake_test")
kind("ConsoleApp")
language("C++")
files({ "premake_test.cc" })
targetdir("bin/%{cfg.buildcfg}")
objdir("obj/%{cfg.buildcfg}")
linkoptions({
	"-L../../nexilis/bin/Debug/",
	"-l:libnexilis-premake_d.so",
	-- The client headers pull in header-only boost.asio TLS code that
	-- references OpenSSL symbols.
	"-lcrypto",
	"-lssl",
	-- Find libnexilis-premake_d.so next to the executable.
	"-Wl,-rpath,'$$ORIGIN'",
})
local boost_includes = os.matchdirs("../../nexilis/third-party/boost/libs/*/include")
table.insert(boost_includes, "../../nexilis/third-party/boost/libs/numeric/conversion/include")
includedirs({
	"../../nexilis/include",
	boost_includes,
})

postbuildcommands({
	"cp ../../nexilis/bin/Debug/libnexilis-premake_d.so bin/Debug/",
	"chmod +x bin/Debug/libnexilis-premake_d.so",
	"echo 'Library ready in bin/Debug/'",
})
