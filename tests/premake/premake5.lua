workspace "NexilisTest"
    configurations { "Debug" }
    architecture "x64"

    project "premake_test"
        kind "ConsoleApp"
        language "C++"
        files { "premake_test.cc" }
        linkoptions {
            "-L../../nexilis/bin/Debug/",
            "-L../../bindings/lua/bin/Debug/",
            "-l:libnexilis-premake_d.so",
            "-l:libnexilis-lua.a",
            "-llua"
        }
        includedirs { 
            "../../nexilis/include",
            "/usr/include/lua5.2"
        }

        postbuildcommands {
            "cp ../../nexilis/bin/Debug/libnexilis-premake_d.so bin/Debug/",
            "chmod +x bin/Debug/libnexilis-premake_d.so",
            "echo 'Library ready in bin/Debug/'"
        }
