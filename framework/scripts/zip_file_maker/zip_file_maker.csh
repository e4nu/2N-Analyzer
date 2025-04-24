#!/bin/csh

# To run:
# source ./framework/scripts/zip_file_maker/zip_file_maker.csh my_custom_archive.zip

# Check and set COLOR_START and COLOR_END if not already defined
if (! $?COLOR_START) then
    unset COLOR_START
    setenv COLOR_START '\033[35m'
endif

if (! $?COLOR_END) then
    unset COLOR_END
    setenv COLOR_END '\033[0m'
endif

echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Running zip maker script                                            =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo 

# Set the base directory to search
set BASE_DIR = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output"

# Set the output zip file path
set OUTPUT_ZIP = "${BASE_DIR}/all_collected_zips.zip"

# Check for optional custom output zip filename
if ($#argv >= 1) then
    set CUSTOM_ZIP_NAME = "$1"
    set OUTPUT_ZIP = "${BASE_DIR}/${CUSTOM_ZIP_NAME}"
    echo "\033[35mCustom output zip filename:\033[0m ${CUSTOM_ZIP_NAME}"
    echo
endif

# Remove any previous version of the output zip
if (-e "${OUTPUT_ZIP}") then
    echo "\033[35mRemoving old\033[0m ${OUTPUT_ZIP}"
    rm "${OUTPUT_ZIP}"
endif

# Find all .zip files in subdirectories and add them to the output zip
echo "\033[35mSearching for .zip files in subdirectories of\033[0m ${BASE_DIR}\033[35m...\033[0m"

if ($#argv >= 1) then
    foreach matchdir (`find "${BASE_DIR}" -mindepth 2 -type d`)
        if ("$matchdir" =~ *${CUSTOM_ZIP_NAME}*) then
            echo "\033[35mMatched directory:\033[0m $matchdir"
            foreach zipfile (`find "${matchdir}" -maxdepth 1 -type f -name "*.zip"`)
                if (-e "${zipfile}") then
                    echo "\033[35m  Found zip:\033[0m ${zipfile}"
                else
                    echo "\033[31m  Zip file not found or inaccessible:\033[0m ${zipfile}"
                endif
                echo "\033[35mAdding\033[0m ${zipfile}"
                zip -j "${OUTPUT_ZIP}" "${zipfile}"
                echo
            end
        endif
    end
else
    foreach zipfile (`find "${BASE_DIR}" -mindepth 2 -type f -name "*.zip"`)
        echo "\033[35mAdding\033[0m ${zipfile}"
        zip -j "${OUTPUT_ZIP}" "${zipfile}"
        echo
    end
endif

echo "\033[35mDone. Final zip:\033[0m ${OUTPUT_ZIP}"
echo
