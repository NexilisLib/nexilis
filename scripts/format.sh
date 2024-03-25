#!/bin/bash

# Define the directories to run Clang-Format recursively.
directories=("../nexilis" "../client" "../server" "../tests")

# Run Clang-Format recursively in the specified directories.
for directory in "${directories[@]}"; do
    echo "Formatting files in directory: $directory"
    find "$directory" -type f \( -name "*.hh" -or -name "*.cc" \) -exec clang-format -i {} +
done

echo "Clang-Format completed."
