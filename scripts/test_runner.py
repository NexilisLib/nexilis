#!/usr/bin/env python3

from create_env import get_nexilis_root
from run_premake import build_and_run_premake

import os
import subprocess
import argparse

def install_dependencies_ubuntu():
    subprocess.run(
        ["sudo", "apt-get", "install", "cmake", "g++", "gcc", "libgtest-dev" ],
        check=True,
    )

def install_dependencies_arch():
    subprocess.run(
        ["sudo", "pacman", "-S", "gcc", "cmake", "gtest", "dotnet-sdk", "premake5" ],
        check=True,
    )

def setup():
    print("Compiling nexilis sources.")
    os.chdir(get_nexilis_root() + "/nexilis")
    if os.path.exists("build"):
        subprocess.run(["rm", "-rf", "build"], check=True)

    os.mkdir("build")
    os.chdir("build")

    subprocess.run(["cmake", ".."], check=True)
    subprocess.run(["make", "-j", str(os.cpu_count())], check=True)
    print("Nexilis sources compiled.")

def run_cpp_tests():
    print("Setting up Nexilis C++ tests...")

    os.chdir(get_nexilis_root() + "/tests/nexilis")
    if os.path.exists("build"):
        subprocess.run(["rm", "-rf", "build"], check=True)

    os.mkdir("build")
    os.chdir("build")

    subprocess.run(["cmake", ".."], check=True)
    subprocess.run(["make", "-j", str(os.cpu_count())], check=True)

    print("Running C++ tests...")
    subprocess.run(["./nexilis_tests"], check=True)
    print("C++ tests completed.")

def run_c_tests():
    print("Setting up Nexilis C tests...")

    os.chdir(get_nexilis_root() + "/tests/nexilisc")

    if os.path.exists("build"):
        subprocess.run(["rm", "-rf", "build"], check=True)

    os.mkdir("build")
    os.chdir("build")

    subprocess.run(["cmake", ".."], check=True)
    subprocess.run(["make", "-j", str(os.cpu_count())], check=True)
    print("Running Nexilis C tests...")
    subprocess.run(["./nexilis_c_tests"], check=True)
    print("C tests completed.")

def run_csharp_tests():
    print("Setting up Nexilis csharp tests...")
    os.chdir(get_nexilis_root() + "/bindings/csharp")
    subprocess.run(["dotnet", "build"], check=True)

    print("Running Nexilis C# tests...")
    os.chdir(get_nexilis_root() + "/tests/CSharpBindings.Tests")
    subprocess.run(["dotnet", "test"], check=True)
    print("C# tests completed.")

def main():
    parser = argparse.ArgumentParser(description="Run tests for the Nexilis project.")
    parser.add_argument(
        "--all", action="store_true", help="Run all project tests."
    )
    parser.add_argument(
        "--unit", action="store_true", help="Run all unit tests."
    )
    parser.add_argument(
        "--cpp", action="store_true", help="Run only C++ tests."
    )
    parser.add_argument(
        "--c", action="store_true", help="Run only C tests."
    )
    parser.add_argument(
        "--csharp", action="store_true", help="Run C# tests."
    )
    parser.add_argument(
        "--premake5", action="store_true", help="Run premake5 testing."
    )
    parser.add_argument(
        "--ubuntu", action="store_true", help="Install dependencies for Ubuntu."
    )
    parser.add_argument(
        "--arch", action="store_true", help="Install dependencies for Arch Linux."
    )

    args = parser.parse_args()

    if not any([args.all, args.unit, args.cpp, args.c, args.csharp, args.premake5, args.arch, args.ubuntu]):
        parser.print_help()
        return

    if args.arch:
        install_dependencies_arch()

    if args.ubuntu:
        install_dependencies_ubuntu()

    if args.all or args.unit or args.cpp or args.c or args.csharp:
        setup()

    if args.all or args.unit or args.cpp:
        run_cpp_tests()
    if args.all or args.unit or args.c:
        run_c_tests()
    if args.all or args.unit or args.csharp:
        run_csharp_tests()
    if args.all or args.premake5:
        build_and_run_premake()

    print("All tests completed successfully!")

if __name__ == "__main__":
    main()
