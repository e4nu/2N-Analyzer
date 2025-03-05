#!/bin/tcsh

echo "${COLOR_START}- Setting screen directory --------------------------------------------${COLOR_END}"
echo ""

unset SCREEN_SCRIPT_DIR
setenv SCREEN_SCRIPT_DIR ${DIR_2N_ANALYSIS_CODE}/setup/scripts/screen
echo "${COLOR_START}SCREEN_SCRIPT_DIR:${COLOR_END} ${SCREEN_SCRIPT_DIR}"
echo ""

# echo "${COLOR_START}- Setting screen aliases ----------------------------------------------${COLOR_END}"
# echo ""

# alias kill_all1 "screen -ls | grep Detached | cut -d. -f1 | awk '{print \$1}' | xargs kill"
# echo "${COLOR_START}kill_all1:${COLOR_END} `alias kill_all1`"
# alias kill_all2 "screen -ls | grep -E '^\s*[0-9]+' | awk '{print \$1}' | cut -d. -f1 | xargs -r -I {} screen -S {} -X quit"
# echo "${COLOR_START}kill_all2:${COLOR_END} `alias kill_all2`"
# echo ""

source $SCREEN_SCRIPT_DIR/setup_screen_aliases.csh

echo "${COLOR_START}- Setting screen command ----------------------------------------------${COLOR_END}"
echo ""

chmod +x $SCREEN_SCRIPT_DIR/start_screen_session.csh
# chmod +x ./setup/scripts/screen/start_screen_session.csh
# source ./setup/scripts/screen/start_screen_session.csh

echo "${COLOR_START}Screen commands have been updated.${COLOR_END}"
echo "${COLOR_START}To run a screen session, use the following command:${COLOR_END}"
echo '$SCREEN_SCRIPT_DIR/start_screen_session.csh <session_name> <command> [stay_open]'
echo ""