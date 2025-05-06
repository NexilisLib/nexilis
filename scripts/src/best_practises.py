from env import get_nexilis_root

import re
import os

# This program checks if the input file/directory has correct usage for type aliases.
# If unused type aliases found, the program asks to write them.
nexilis_root = get_nexilis_root()

# Header file that contains type alias declarations.
aliases_file_path = nexilis_root + '/nexilis/include/nexilis/nexilis_constants.hh'

# Read the aliases from the aliases file.
def read_aliases(file_path):
    alias_map = {}
    with open(file_path, 'r') as file:
        for line in file:
            # Match using type alias lines (e.g., using nx_data = std::vector<uint8_t>;).
            using_match = re.match(r'using\s+(\w+)\s+=\s+(.+);', line)
            if using_match:
                alias = using_match.group(1)
                value = using_match.group(2)
                alias_map[value] = alias
    return alias_map

# Analyze the C++ file for type aliases.
def analyze_cpp_file(file_path, alias_map):
    # Skip alias file itself.
    if file_path == aliases_file_path:
        return 0, []

    with open(file_path, 'r') as file:
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
    with open(file_path, 'r') as file:
        content = file.read()

    # Replace each value with its corresponding alias.
    for value, alias in alias_map.items():
        content = re.sub(re.escape(value), alias, content)

    # Write the changes back to the source file.
    with open(file_path, 'w') as file:
        file.write(content)

# Handle both files and directories.
def process_files(target_path, alias_map):
    total_replacements = 0
    all_changes = []

    # If the target is a single file, process it directly
    if os.path.isfile(target_path):
        replacements, changes = analyze_cpp_file(target_path, alias_map)
        total_replacements += replacements
        all_changes.extend([(target_path, value, alias, count) for value, alias, count in changes])

    # If the target is a directory, walk through all the files
    elif os.path.isdir(target_path):
        for root, dirs, files in os.walk(target_path):
            for file in files:
                if file.endswith(('.cc', '.hh')):
                    file_path = os.path.join(root, file)
                    replacements, changes = analyze_cpp_file(file_path, alias_map)
                    total_replacements += replacements
                    all_changes.extend([(file_path, value, alias, count) for value, alias, count in changes])

    return total_replacements, all_changes

def main():
    # Read the aliases from the aliases file.
    alias_map = read_aliases(aliases_file_path)

    # Analyze the C++ file and get potential replacements.
    total_replacements, changes = process_files(nexilis_root + "/nexilis", alias_map)

    if total_replacements == 0:
        print("0")
        return

    # Print all potential changes
    print(f"\nTotal potential replacements: {total_replacements}")
    for file_path, value, alias, count in changes:
        print(f"File: {file_path} | Value: '{value}' -> Alias: '{alias}', Occurrences: {count}")

    # Ask the user if they want to write the changes
    print(f"Total replacements: {total_replacements}")
    response = input(f"Do you want to write changes? (y/n):").strip().lower()

    if response == 'y':
        for file_path, _, __, ___ in changes:
            apply_replacements(file_path, alias_map)
        print("Changes written.")
    else:
        print("No changes made.")

if __name__ == "__main__":
    main()
