import os
import subprocess

# Get the directory of the script.
script_dir = os.path.dirname(os.path.abspath(__file__))

# Define the directories to run Clang-Format recursively.
directories = ["nexilis", "examples", "tests"]

# Run Clang-Format recursively in the specified directories.
for directory in directories:
    target_dir = os.path.abspath(os.path.join(script_dir, "..", directory))

    if os.path.isdir(target_dir):
        print(f"Formatting files in directory: {target_dir}")
        find_command = (
            rf'find "{target_dir}" -type f \( -name "*.hh" -o -name "*.cc" \) -exec clang-format -i {{}} +'
        )
        subprocess.run(find_command, shell=True, check=True)
    else:
        print(f"Skipping: {target_dir} (Directory not found)")

print("Clang-Format completed.")
