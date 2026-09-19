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
import json
import sys


# Format single JSON file.
def format_json_file(file_path):
    try:
        with open(file_path, "r+") as f:
            json_data = json.load(f)
            f.seek(0)

            # Pretty-print the JSON data.
            formatted_json = json.dumps(json_data, indent=4)

            # Write the formatted JSON back to the file.
            f.write(formatted_json)
            f.truncate()

        print("JSON file formatted successfully.")

    except FileNotFoundError:
        print("File not found.")
    except json.JSONDecodeError:
        print("Invalid JSON format in the file.")
    except Exception as e:
        print("An error occurred:", e)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 format_json_file.py <file_path>")
    else:
        file_path = sys.argv[1]
        format_json_file(file_path)


# Format JSON files in a directory.
def format_json_files(directory):
    # Get list of files in the directory.
    files = os.listdir(directory)

    # Iterate over each file.
    for file in files:
        # Check if file ends with .json
        if file.endswith(".json"):
            # Format file.
            format_json_file(os.path.join(directory, file))


# Specify the directory containing JSON files.
directory_path = "."

# Call the function to format JSON files in the directory.
format_json_files(directory_path)
