from env import get_nexilis_root

import os
import subprocess
import sys

# Define the directories to run Clang-Format recursively.
directories = ["nexilis", "examples", "tests"]
changes_detected = False

# Run Clang-Format recursively in the specified directories.
for directory in directories:
    target_dir = get_nexilis_root()
    if os.path.isdir(target_dir):
        print(f"Formatting files in directory: {target_dir}")

        # Check if files need formatting.
        find_check_command = (
            rf'find "{target_dir}" -type f \( -name "*.hh" -o -name "*.cc" \) '
            r'-exec clang-format --dry-run --Werror {} +'
        )
        result = subprocess.run(find_check_command, shell=True, capture_output=True, text=True)

        if result.returncode != 0:
            changes_detected = True
            # Actually apply formatting
            find_apply_command = (
                rf'find "{target_dir}" -type f \( -name "*.hh" -o -name "*.cc" \) '
                r'-exec clang-format -i {} +'
            )
            subprocess.run(find_apply_command, shell=True, check=True)
            print(f"Formatted files in {target_dir}")
    else:
        print(f"Skipping: {target_dir} (Directory not found)")

if changes_detected:
    print("::error::Formatting changes were required and have been applied")
    sys.exit(1)
else:
    print("No formatting changes needed")
    sys.exit(0)
