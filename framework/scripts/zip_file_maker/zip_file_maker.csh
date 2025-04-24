#!/bin/csh

# To run:
# source ./framework/scripts/zip_file_maker/zip_file_maker.csh V14_custom_plots_path_prefix_test

# Set colors if not already set
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

# Check input argument
if ($#argv < 1) then
    echo "${COLOR_START}Error:${COLOR_END} Must provide base name to match directories."
    exit 1
endif

# Define input and output
set NAME_FOR_MATCHING = "$1"
set BASE_DIR = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output"
set OUTPUT_ZIP = "${BASE_DIR}/${NAME_FOR_MATCHING}.zip"

echo "${COLOR_START}Looking for zip files in:${COLOR_END} ${BASE_DIR}/*${NAME_FOR_MATCHING}*/"
echo "${COLOR_START}Saving to:${COLOR_END} ${OUTPUT_ZIP}"
echo

# Remove previous output zip if it exists
if (-e "${OUTPUT_ZIP}") then
    echo "${COLOR_START}Removing existing zip:${COLOR_END} ${OUTPUT_ZIP}"
    rm "${OUTPUT_ZIP}"
endif

# Loop over matching directories
foreach dir (`find "${BASE_DIR}" -maxdepth 1 -mindepth 1 -type d -name "*${NAME_FOR_MATCHING}*"`)
    echo "${COLOR_START}Matched directory:${COLOR_END} $dir"
    set dir_name = `basename "$dir"`
    set zip_path = "$dir/$dir_name.zip"

    if (-e "$zip_path") then
        echo "${COLOR_START}  Found zip file:${COLOR_END} $zip_path"
        zip -j "$OUTPUT_ZIP" "$zip_path"
    else
        echo "${COLOR_END}  ⚠️  No zip found at:${COLOR_END} $zip_path"
    endif
end

echo
echo "${COLOR_START}Done. Final zip:${COLOR_END} ${OUTPUT_ZIP}"
echo