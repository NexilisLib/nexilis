#!/bin/bash

# Get the directory of the script.
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define the directories to run Clang-Format recursively.
directories=("nexilis" "examples" "tests")

# Run Clang-Format recursively in the specified directories.
for directory in "${directories[@]}"; do
    target_dir="$script_dir/../$directory"
    if [ -d "$target_dir" ]; then
        echo "Formatting files in directory: $target_dir"
        find "$target_dir" -type f \( -name "*.hh" -or -name "*.cc" \) -exec clang-format -i {} +
    else
        echo "Skipping: $target_dir (Directory not found)" 
    fi
done

echo "Clang-Format completed."
