import os

# Function to format JSON files in a directory.
def format_json_files(directory):
    # Get list of files in the directory.
    files = os.listdir(directory)

    # Iterate over each file.
    for file in files:
        # Check if file ends with .json
        if file.endswith('.json'):
            # Get full path of the file.
            file_path = os.path.join(directory, file)

            # Call format_json_file.py script with file path as argument.
            os.system(f"python3 format_json_file.py {file_path}")

# Specify the directory containing JSON files.
directory_path = '.'

# Call the function to format JSON files in the directory.
format_json_files(directory_path)
