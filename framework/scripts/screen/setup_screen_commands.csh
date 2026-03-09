#!/bin/tcsh

# This script sets up the environment variables for the 2N-Analyzer code. It updates the environment by setting the DIR_2N_ANALYSIS_CODE variable to the current working directory, and it also sets the ANALYSIS_HOSTNAME variable to the hostname of the machine. Additionally, it checks if the hostname contains a specific substring (defined in JLAB_TESTER) and sets the IFARM_RUN variable accordingly. The script also uses color codes to make the output more visually appealing.
# Note: The color codes are defined using ANSI escape sequences, and they are used to highlight the output in the terminal. The DIR_2N_ANALYSIS_CODE variable is set to the current working directory, which allows the code to reference files and directories relative to the location of the script. The ANALYSIS_HOSTNAME variable is set to the hostname of the machine, which can be useful for determining whether certain commands should be run based on the environment. The IFARM_RUN variable is set based on whether the hostname contains a specific substring, which allows for conditional execution of commands based on the environment.

echo "${COLOR_START}===========================================================================================${COLOR_END}"
echo "${COLOR_START}= Setting up screen functions                                                             =${COLOR_END}"
echo "${COLOR_START}===========================================================================================${COLOR_END}"
echo ""

echo "${COLOR_START}- Setting screen directory ----------------------------------------------------------------${COLOR_END}"
echo ""

unset SCREEN_SCRIPT_DIR
setenv SCREEN_SCRIPT_DIR ${DIR_2N_ANALYSIS_CODE}/framework/scripts/screen
echo "${COLOR_START}SCREEN_SCRIPT_DIR:${COLOR_END} ${SCREEN_SCRIPT_DIR}"
echo ""

# echo "${COLOR_START}- Setting screen aliases ------------------------------------------------------------------${COLOR_END}"
# echo ""

# alias kill_all1 "screen -ls | grep Detached | cut -d. -f1 | awk '{print \$1}' | xargs kill"
# echo "${COLOR_START}kill_all1:${COLOR_END} `alias kill_all1`"
# alias kill_all2 "screen -ls | grep -E '^\s*[0-9]+' | awk '{print \$1}' | cut -d. -f1 | xargs -r -I {} screen -S {} -X quit"
# echo "${COLOR_START}kill_all2:${COLOR_END} `alias kill_all2`"
# echo ""

source $SCREEN_SCRIPT_DIR/setup_screen_aliases.csh

echo "${COLOR_START}- Setting screen command ------------------------------------------------------------------${COLOR_END}"
echo ""

chmod +x $SCREEN_SCRIPT_DIR/start_screen_session.csh
# chmod +x ./framework/scripts/screen/start_screen_session.csh
# source ./framework/scripts/screen/start_screen_session.csh

echo "${COLOR_START}Screen commands have been updated.${COLOR_END}"
echo "${COLOR_START}To run a screen session, use the following command:${COLOR_END}"
echo '$SCREEN_SCRIPT_DIR/start_screen_session.csh <session_name> <command> [stay_open]'
echo ""