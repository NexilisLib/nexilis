#!/usr/bin/env python3

import sys
import argparse

from env import create_env_file, get_nexilis_root
from format import format_all_files
from cppcheck import run_cppcheck, get_nexilis_dirs
from test_runner import install, run_cpp_tests, run_c_tests, run_csharp_tests
from best_practises import process_all_files, read_aliases, get_constants_file_path


def run_minimal_checks():
    nexilis_root = get_nexilis_root()
    format_result = format_all_files()
    if format_result != 0:
        print("Some checks failed.")
        sys.exit(1)
    else:
        print("Format check passed.")

    aliases = read_aliases(get_constants_file_path(nexilis_root))
    best_practises = process_all_files(aliases)
    if best_practises != 0:
        print("Something wrong with best practices check")
        sys.exit(1)
    else:
        print("All best practice checks passed.")

    include_directory, src_directory, exclude_directories = get_nexilis_dirs(
        nexilis_root
    )
    cppcheck_result = run_cppcheck(
        include_dir=include_directory,
        src_dir=src_directory,
        exclude_dirs=exclude_directories,
        strict=False,
    )

    print(cppcheck_result)
    if cppcheck_result:
        print("Cppcheck passed.")
    else:
        print("Cppcheck failed.")
        sys.exit(1)


def run_tests():
    install()
    run_cpp_tests()
    run_c_tests()
    run_csharp_tests()


def main():
    parser = argparse.ArgumentParser(
        description="Pre-commit script to run various checks."
    )

    parser.add_argument("--all", action="store_true", help="Run all checks.")
    parser.add_argument("--minimal", action="store_true", help="Run minimal checks.")
    parser.add_argument("--tests", action="store_true", help="Run all tests.")
    args = parser.parse_args()

    create_env_file()

    if args.all:
        run_minimal_checks()
        run_tests()
    elif args.minimal:
        run_minimal_checks()
    elif args.tests:
        run_tests()
    else:
        parser.print_help()
        sys.exit(1)


if __name__ == "__main__":
    main()
