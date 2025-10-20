#!/usr/bin/env python3
import sys
import argparse
from pre_commit.run_command import run_command

def create_env():
    res_env = run_command("python scripts/pre_commit/env.py")
    if res_env.returncode != 0:
        print("Failed to set up environment.")
        sys.exit(1)
    else:
        print("Env set up successfully.")

def run_minimal_checks():
    res_format = run_command("python scripts/pre_commit/format.py")
    if res_format.returncode != 0:
        print("Code formatting issues detected.")
        sys.exit(1)
    else:
        print("Code formatted successfully.")

    res_cppcheck = run_command("python scripts/pre_commit/cppcheck.py")
    if res_cppcheck.returncode != 0:
        print("Cppcheck found issues.")
        sys.exit(1)
    else:
        print("Cppcheck passed successfully.")

    res_best_practices = run_command("python scripts/pre_commit/best_practices.py")
    if res_best_practices.returncode != 0:
        print("Best practices check failed.")
        sys.exit(1)
    else:
        print("Best practices check passed successfully.")

def building_add_testing():
    res_premake = run_command("python scripts/pre_commit/run_premake.py")
    if res_premake.returncode != 0:
        print("Premake build failed.")
        sys.exit(1)
    else:
        print("Premake build succeeded.")

    res_tests_nexilis = run_command("python scripts/pre_commit/test_runner.py --cpp")
    if res_tests_nexilis.returncode != 0:
        print("Nexilis tests failed.")
        sys.exit(1)
    else:
        print("Nexilis tests passed successfully.")

    res_tests_nexilisc = run_command("python scripts/pre_commit/test_runner.py --c")
    if res_tests_nexilisc.returncode != 0:
        print("Nexilisc tests failed.")
        sys.exit(1)
    else:
        print("Nexilisc tests passed successfully.")

    res_tests_csharp = run_command("python scripts/pre_commit/test_runner.py --csharp")
    if res_tests_csharp.returncode != 0:
        print("Nexilis C# tests failed.")
        sys.exit(1)
    else:
        print("Nexilis C# tests passed successfully.")

        sys.exit(0)


def main():
    parser = argparse.ArgumentParser(description="Pre-commit script to run various checks.")

    parser.add_argument(
        "--all", action="store_true", help="Run all checks."
    )
    parser.add_argument(
        "--minimal", action="store_true", help="Run minimal checks."
    )
    parser.add_argument(
        "--tests", action="store_true", help="Run all tests."
    )

    args = parser.parse_args()

    create_env()

    if args.all:
        run_minimal_checks()
        building_add_testing()
    elif args.minimal:
        run_minimal_checks()
    elif args.tests:
        building_add_testing()
    else:
        parser.print_help()
        sys.exit(1)
    
if __name__ == "__main__":
    main()

