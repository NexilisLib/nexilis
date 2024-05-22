#!/bin/bash

# Dependencies: dmenu

# Function to display confirmation dialog using dmenu
confirm_delete()
{
    # Prompt user for confirmation
    confirmed=$(echo -e "Yes\nNo" | dmenu -p "Are you sure you want to delete logs?")

    # Check user's choice
    if [[ $confirmed == "Yes" ]]; then
        return 0 # Confirmation received
    else
        return 1 # Confirmation denied
    fi
}

# Get a list of files ending with .json or .txt
files=$(ls *.json *.txt 2>/dev/null)

# Check if any files exist
if [[ -n $files ]]; then
    # Display files to the user
    echo "Files to be deleted:"
    echo "$files"

    # Ask for confirmation
    if confirm_delete; then
        # Delete files if confirmed
        rm -f *.json *.txt
        echo "Files deleted."
    else
        echo "Deletion aborted."
    fi
else
    echo "No .json or .txt files found in the current directory."
fi
