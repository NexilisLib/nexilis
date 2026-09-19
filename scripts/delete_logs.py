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

import os
import glob
import sys


def confirm_delete():
    confirmed = (
        input("Are you sure you want to delete logs? (yes/no): ").strip().lower()
    )
    return confirmed == "yes"


# Get directory from command-line arguments (default to current directory)
directory = sys.argv[1] if len(sys.argv) > 1 else "."

if not os.path.isdir(directory):
    print(f"Error: Directory '{directory}' not found.")
    sys.exit(1)


def join(file_extension):
    os.path.join(directory, file_extension)


files = glob.glob(join("*.json")) + glob.glob(join("*.txt"))

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
