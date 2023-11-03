#!/bin/bash

# Define the source file and destination directory on the external drive
source_file="build/kernel8.img"
destination_directory="/Volumes/bootfs"

# Check if the source file exists
if [ ! -f "$source_file" ]; then
    echo "Source file does not exist: $source_file"
    exit 1
fi

# Check if the destination directory exists and is writable
if [ ! -d "$destination_directory" ] || [ ! -w "$destination_directory" ]; then
    echo "Destination directory does not exist or is not writable: $destination_directory"
    exit 1
fi

# Copy the file to the external drive
cp "$source_file" "$destination_directory"

# Check the exit status of the cp command
if [ $? -eq 0 ]; then
    echo "File copied successfully to $destination_directory"
else
    echo "Error copying the file"
fi
