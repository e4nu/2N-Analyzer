#!/bin/csh

echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Running zip maker script                                            =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo 

# Set the base directory to search
set BASE_DIR = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output"

# Set the output zip file path
set OUTPUT_ZIP = "$BASE_DIR/all_collected_zips.zip"

# Remove any previous version of the output zip
if (-e "$OUTPUT_ZIP") then
    echo "\033[35mRemoving old $OUTPUT_ZIP\033[0m"
    rm "$OUTPUT_ZIP"
endif

# Find all .zip files in subdirectories and add them to the output zip
echo "\033[35mSearching for .zip files in subdirectories of $BASE_DIR...\033[0m"

foreach zipfile (`find "$BASE_DIR" -mindepth 2 -type f -name "*.zip"`)
    echo "\033[35mAdding $zipfile\033[0m"
    zip -j "$OUTPUT_ZIP" "$zipfile"
end

echo "\033[35mDone. Final zip: $OUTPUT_ZIP\033[0m"
echo
