#!/usr/bin/env python3

from create_env import get_nexilis_root

import os
import shutil
import fnmatch

def read_gitignore(gitignore_path):
    """Read the .gitignore file and return a list of patterns."""
    patterns = []
    if os.path.isfile(gitignore_path):
        with open(gitignore_path, 'r') as file:
            for line in file:
                line = line.strip()
                if line and not line.startswith('#'):  # Ignore comments and empty lines.
                    patterns.append(line)
    return patterns

def matches_any_pattern(path, patterns):
    """Check if the given path matches any of the patterns in .gitignore."""
    for pattern in patterns:
        if fnmatch.fnmatch(path, pattern) or fnmatch.fnmatch(path, f"*/{pattern}"):
            return True
    return False

def delete_ignored_files(base_dir, patterns):
    """Delete files and directories listed in .gitignore."""
    for root, dirs, files in os.walk(base_dir, topdown=False):
        # Delete files
        for file in files:
            file_path = os.path.join(root, file)
            relative_path = os.path.relpath(file_path, base_dir)
            if matches_any_pattern(relative_path, patterns) and file != ".env":
                print(f"Deleting file: {file_path}")
                os.remove(file_path)

        # Delete directories
        for dir in dirs:
            dir_path = os.path.join(root, dir)
            relative_path = os.path.relpath(dir_path, base_dir)
            if matches_any_pattern(relative_path, patterns):
                print(f"Deleting directory: {dir_path}")
                shutil.rmtree(dir_path)

def prompt_for_env_deletion(base_dir):
    """Ask the user if they want to delete the .env file."""
    env_path = os.path.join(base_dir, ".env")
    if os.path.isfile(env_path):
        response = input("Do you want to delete the .env file? (y/n): ").strip().lower()
        if response == 'y':
            print(f"Deleting .env file: {env_path}")
            os.remove(env_path)
        else:
            print(".env file was not deleted.")

def main():
    # Locale nexilis root .gitignore, TODO add support for other .gitignore files.
    base_dir = get_nexilis_root()
    gitignore_path = os.path.join(base_dir, '.gitignore')

    # Read .gitignore patterns.
    patterns = read_gitignore(gitignore_path)
    if not patterns:
        print("No patterns found in .gitignore.")
        return

    # Delete files and directories matching the patterns.
    delete_ignored_files(base_dir, patterns)

    # Prompt the user to delete the .env file.
    prompt_for_env_deletion(base_dir)
    print("Cleanup complete.")

if __name__ == "__main__":
    main()