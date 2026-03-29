#!/bin/csh

# To run:
# source ./scripts/zip_file_maker/zip_file_maker.csh

echo "${COLOR_START}===========================================================================================${COLOR_END}"
echo "${COLOR_START}= Running zip maker script                                                                =${COLOR_END}"
echo "${COLOR_START}===========================================================================================${COLOR_END}"
echo 

# Set the base directory to search
set BASE_DIR = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output"

# Set the output zip file path
set OUTPUT_ZIP = "${BASE_DIR}/all_collected_zips.zip"

# Remove any previous version of the output zip
if (-e "$OUTPUT_ZIP") then
    echo "${COLOR_START}Removing old${COLOR_END} $OUTPUT_ZIP"
    rm "$OUTPUT_ZIP"
endif

# Find all .zip files in subdirectories and add them to the output zip
echo "${COLOR_START}Searching for .zip files in subdirectories of${COLOR_END} $BASE_DIR${COLOR_START}...${COLOR_END}"

foreach zipfile (`find "${BASE_DIR}" -mindepth 2 -type f -name "*.zip"`)
    echo "${COLOR_START}Adding${COLOR_END} ${zipfile}"
    zip -j "${OUTPUT_ZIP}" "${zipfile}"
    echo
end

echo "${COLOR_START}Done. Final zip:${COLOR_END} ${OUTPUT_ZIP}"
echo
