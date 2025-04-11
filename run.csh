#!/bin/tcsh

# Lunch the update script to update the repository and clean excessive files.
source update_only.sh

echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Update review                                                       =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo ""

# Display the latest commit in the current branch. The -1 option limits the output to one commit, and the
# --oneline option formats the output to show only the commit hash and the commit message in a single line.
# This command is useful for quickly checking the latest commit in the current branch without displaying
# the full commit history.

echo "${COLOR_START}HEAD:${COLOR_END}"
git log -1 --oneline
echo ""

echo "\033[35m- Launching data-analysis code -------------------------------------\033[0m"
echo ""

# Launch the data-analysis code using the clas12root command. The -b option is used to run the code in batch
# mode, -q option quits clas12root after operation.

# TODO: compile the code with cmake instead of using this command (which is better for small macros)

clas12root -b -q ./main.cpp
echo ""

echo "\033[35m- Operation finished --------------------------------------------------\033[0m"
echo ""
