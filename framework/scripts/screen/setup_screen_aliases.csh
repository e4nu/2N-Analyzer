#!/bin/tcsh

# This script sets up the screen aliases for the 2N-Analyzer code. It defines several aliases for managing screen sessions, including killing all detached screen sessions, killing all screen sessions, and starting new screen sessions with specific commands. The script also uses color codes to make the output more visually appealing.
# Note: The color codes are defined using ANSI escape sequences, and they are used to highlight the output in the terminal. The aliases defined in this script allow for easy management of screen sessions, including killing all detached sessions, killing all sessions, and starting new sessions with specific commands. The aliases can be used in the terminal to quickly execute the corresponding commands, and they are designed to be easily remembered and used for common screen management tasks. It is important to use these aliases appropriately based on the goals of the analysis and the characteristics of the environment in which the code is being run. Additionally, it is important to ensure that the screen sessions are managed properly to avoid issues with multiple sessions running simultaneously or with orphaned sessions that are not properly closed.

echo "${COLOR_START}- Setting screen aliases ------------------------------------------------------------------${COLOR_END}"
echo ""

alias setup_screen_aliases "source ${DIR_2N_ANALYSIS_CODE}/framework/scripts/screen/setup_screen_aliases.csh"
echo "${COLOR_START}setup_screen_aliases:${COLOR_END} `alias setup_screen_aliases`"
alias kill_all1 "screen -ls | grep Detached | cut -d. -f1 | awk '{print \$1}' | xargs kill"
echo "${COLOR_START}kill_all1:${COLOR_END} `alias kill_all1`"
alias kill_all2 "screen -ls | grep -E '^\s*[0-9]+' | awk '{print \$1}' | cut -d. -f1 | xargs -r -I {} screen -S {} -X quit"
echo "${COLOR_START}kill_all2:${COLOR_END} `alias kill_all2`"
alias crunc "$SCREEN_SCRIPT_DIR/start_screen_session.csh '' 'source run.sh'"
echo "${COLOR_START}crunc:${COLOR_END} `alias crunc`"
alias srunnfd "$SCREEN_SCRIPT_DIR/start_screen_session.csh '' 'source nFD_eff_tester_run.sh'"
echo "${COLOR_START}srunnfd:${COLOR_END} `alias srunnfd`"
echo ""
echo "${COLOR_START}Screen aliases have been set.${COLOR_END}"
echo "${COLOR_START}To reset these aliases:${COLOR_END}"
echo 'source $SCREEN_SCRIPT_DIR/setup_screen_aliases.csh'
echo ""