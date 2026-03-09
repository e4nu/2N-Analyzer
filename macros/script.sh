#!/usr/bin/env bash

# Directory containing the files (default: current directory)
DIR="${1:-.}"

for file in "$DIR"/*_AMap_*; do
# for file in "$DIR"/*_WMap_*; do
    # Skip if no matching files
    [ -e "$file" ] || continue

    new_name="${file//_AMap_/_AMaps_}"
    # new_name="${file//_WMap_/_WMaps_}"

    mv "$file" "$new_name"
    echo "Renamed: $file -> $new_name"
done