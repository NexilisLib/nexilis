#!/bin/bash

# Function to display confirmation dialog using read.
confirm_delete() {
    echo -n "Are you sure you want to delete logs? (yes/no): "
    read -r confirmed

    # Convert input to lowercase for consistency.
    confirmed=$(echo "$confirmed" | tr '[:upper:]' '[:lower:]')

    if [[ $confirmed == "yes" ]]; then
        return 0  # Confirmation received.
    else
        return 1  # Confirmation denied.
    fi
}

# Get a list of files ending with .json or .txt
files=$(ls *.json *.txt 2>/dev/null)

# Check if any files exist.
if [[ -n $files ]]; then
    # Display files to the user.
    echo "Files to be deleted:"
    echo "$files"

    # Ask for confirmation.
    if confirm_delete; then
        # Delete files if confirmed.
        rm -f *.json *.txt
        echo "Files deleted."
    else
        echo "Deletion aborted."
    fi
else
    echo "No .json or .txt files found in the current directory."
fi
