import os
import glob
import sys

def confirm_delete():
    confirmed = input("Are you sure you want to delete logs? (yes/no): ").strip().lower()
    return confirmed == "yes"

# Get directory from command-line arguments (default to current directory)
directory = sys.argv[1] if len(sys.argv) > 1 else "."

if not os.path.isdir(directory):
    print(f"Error: Directory '{directory}' not found.")
    sys.exit(1)

files = glob.glob(os.path.join(directory, "*.json")) + glob.glob(os.path.join(directory, "*.txt"))

if files:
    print("Files to be deleted:")
    for file in files:
        print(file)

    # Ask for confirmation
    if confirm_delete():
        # Delete files if confirmed
        for file in files:
            os.remove(file)
        print("Files deleted.")
    else:
        print("Deletion aborted.")
else:
    print(f"No .json or .txt files found in '{directory}'.")
