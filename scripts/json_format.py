import os
import json
import sys


# Format single JSON file.
def format_json_file(file_path):
    try:
        with open(file_path, 'r+') as f:
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
        if file.endswith('.json'):
            # Format file.
            format_json_file(os.path.join(directory, file))


# Specify the directory containing JSON files.
directory_path = '.'

# Call the function to format JSON files in the directory.
format_json_files(directory_path)
