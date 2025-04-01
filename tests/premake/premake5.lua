workspace "NexilisTest"
    configurations { "Debug" }
    architecture "x64"

    project "premake_test"
        kind "ConsoleApp"
        language "C++"
        files { "premake_test.cc" }
        linkoptions {
            "-L../../nexilis/bin/Debug/",
            "-l:libnexilis-premake_d.so"
        }
        includedirs { "../../nexilis/include" }

        postbuildcommands {
            "cp ../../nexilis/bin/Debug/libnexilis-premake_d.so bin/Debug/",
            "chmod +x bin/Debug/libnexilis-premake_d.so",
            "echo 'Library ready in bin/Debug/'"
        }
