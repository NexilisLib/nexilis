#!/usr/bin/env python3

from env import get_nexilis_root
from run_premake import build_and_run_premake

import os
import subprocess
import argparse
import sys


def install():
    print("Compiling nexilis sources.")
    os.chdir(get_nexilis_root() + "/nexilis")
    if os.path.exists("build"):
        subprocess.run(["rm", "-rf", "build"], check=True)

    prefix = "-DCMAKE_INSTALL_PREFIX=" + os.path.join(os.getcwd(), "build", "install")
    subprocess.run(["cmake", "-B", "build", prefix], check=True)

    cpu_count = str(os.cpu_count())
    subprocess.run(
        ["cmake", "--build", "build", "--target", "install", "-j", cpu_count],
        check=True,
    )

    print("Nexilis installed successfully.")


def run_cpp_tests():
    print("Setting up Nexilis C++ tests...")

    os.chdir(get_nexilis_root() + "/tests/nexilis")
    if os.path.exists("build"):
        subprocess.run(["rm", "-rf", "build"], check=True)

    os.mkdir("build")
    os.chdir("build")

    subprocess.run(["cmake", "..", "-DNEXILIS_IS_LOCAL=1"], check=True)
    subprocess.run(["cmake", "--build", ".", "-j", str(os.cpu_count())], check=True)

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
    parser.add_argument("--all", action="store_true", help="Run all project tests.")
    parser.add_argument("--cpp", action="store_true", help="Run only C++ tests.")
    parser.add_argument("--c", action="store_true", help="Run only C tests.")
    parser.add_argument("--csharp", action="store_true", help="Run C# tests.")
    parser.add_argument("--premake5", action="store_true", help="Run premake5 testing.")
    args = parser.parse_args()

    if not any([args.all, args.cpp, args.c, args.csharp, args.premake5]):
        parser.print_help()
        return

    if args.all or args.cpp or args.c or args.csharp:
        install()

    if args.all or args.cpp:
        run_cpp_tests()
    if args.all or args.c:
        run_c_tests()
    if args.all or args.csharp:
        run_csharp_tests()
    if args.all or args.premake5:
        success, output = build_and_run_premake()
        print(output)
        if not success:
            print("Premake test failed.")
            sys.exit(1)

    print("\nAll tests completed successfully!")


if __name__ == "__main__":
    main()
