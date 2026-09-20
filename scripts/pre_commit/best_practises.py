#     Copyright (C) 2026 Valtteri Viirret
#     This file is part of the Nexilis Project.
#
#     This file is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Lesser General Public License as
#     published by the Free Software Foundation, either version 3 of the
#     License, or (at your option) any later version.
#
#     This file is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU Lesser General Public License for more details.
#
#     You should have received a copy of the GNU Lesser General Public License
#     along with this file.  If not, see <https://gnu.org>.

# This program checks if the input file/directory has correct usage for type aliases.
# If unused type aliases found, the program asks to write them.

from env import get_nexilis_root

import re
import os
import sys
import argparse


def get_constants_file_path(nexilis_root) -> str:
    return nexilis_root + "/nexilis/include/nexilis/nexilis_constants.hh"


# Read the aliases from the aliases file.
def read_aliases(file_path) -> dict:
    alias_map = {}
    with open(file_path, "r") as file:
        for line in file:
            # Match using type alias lines (e.g., using nx_data = std::vector<uint8_t>;).
            using_match = re.match(r"using\s+(\w+)\s+=\s+(.+);", line)
            if using_match:
                alias = using_match.group(1)
                value = using_match.group(2)
                alias_map[value] = alias
    return alias_map


# Analyze the C++ file for type aliases.
def analyze_cpp_file(file_path, alias_map) -> tuple[int, list[tuple[str, str, int]]]:
    # Skip alias file itself.
    if file_path == get_constants_file_path(get_nexilis_root()):
        return 0, []

    with open(file_path, "r") as file:
        content = file.read()

    total_replacements = 0
    changes = []

    # Check for every alias value in the source file.
    for value, alias in alias_map.items():
        pattern = re.escape(value)
        matches = re.findall(pattern, content)
        if matches:
            count = len(matches)
            total_replacements += count
            changes.append((value, alias, count))

    return total_replacements, changes


# Apply the replacements in the source file.
def apply_replacements(file_path, alias_map):
    with open(file_path, "r") as file:
        content = file.read()

    # Replace each value with its corresponding alias.
    for value, alias in alias_map.items():
        content = re.sub(re.escape(value), alias, content)

    # Write the changes back to the source file.
    with open(file_path, "w") as file:
        file.write(content)


def extend_changes(all_changes, file_path, changes):
    for value, alias, count in changes:
        all_changes.append((file_path, value, alias, count))


# Handle both files and directories.
def process_files(target_path, alias_map) -> tuple[int, list[tuple[str, str, int]]]:
    total_replacements = 0
    all_changes = []

    # If the target is a single file, process it directly
    if os.path.isfile(target_path):
        replacements, changes = analyze_cpp_file(target_path, alias_map)
        total_replacements += replacements
        extend_changes(all_changes, target_path, changes)

    # If the target is a directory, walk through all the files
    elif os.path.isdir(target_path):
        for root, dirs, files in os.walk(target_path):
            for file in files:
                if file.endswith((".cc", ".hh")):
                    file_path = os.path.join(root, file)
                    replacements, changes = analyze_cpp_file(file_path, alias_map)
                    total_replacements += replacements
                    extend_changes(all_changes, file_path, changes)

    return total_replacements, all_changes


def process_all_files(alias_map, apply=False) -> int:
    """Analyze the C++ sources for incorrect type alias usage.

    Returns 0 when the sources are clean or the replacements were applied,
    and a non-zero value when unused type aliases were found but not applied.
    """
    # Analyze the C++ file and get potential replacements.
    path = get_nexilis_root() + "/nexilis"
    total_replacements, changes = process_files(path, alias_map)

    if total_replacements == 0:
        return 0

    # Print all potential changes
    print(f"\nTotal potential replacements: {total_replacements}")
    for file_path, value, alias, count in changes:
        msg = f"File: {file_path} | Value: '{value}' ->"
        msg += f" Alias: '{alias}', Occurrences: {count}"
        print(msg)

    # If asked, apply the replacements without prompting.
    if apply:
        for file_path, _, __, ___ in changes:
            apply_replacements(file_path, alias_map)
        print("Replacements applied.")
        return 0

    # Never prompt when running non-interactively (e.g. in CI).
    if not sys.stdin.isatty():
        print("Unused type aliases found, no changes made.")
        return 1

    # Ask the user if they want to write the changes
    response = input(
        f"Apply {total_replacements} replacement(s)? (y/n):"
    ).strip().lower()
    if response == "y":
        for file_path, _, __, ___ in changes:
            apply_replacements(file_path, alias_map)
        print("Replacements applied.")
        return 0
    else:
        print("No changes made.")
        return 1


def main():
    parser = argparse.ArgumentParser(
        description="Check correct type alias usage in the Nexilis C++ sources."
    )
    parser.add_argument(
        "--apply",
        action="store_true",
        help="Apply the type alias replacements without prompting.",
    )
    args = parser.parse_args()

    result = process_all_files(
        read_aliases(get_constants_file_path(get_nexilis_root())),
        apply=args.apply,
    )
    if result != 0:
        print("Unused type aliases found. Run with --apply to replace them.")
        sys.exit(1)
    print("Everything OK")


if __name__ == "__main__":
    main()
