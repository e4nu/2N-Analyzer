#!/bin/tcsh

echo "${COLOR_START}- Setting screen commands ---------------------------------------------${COLOR_END}"
echo ""

unset SCREEN_SCRIPT_DIR
setenv SCREEN_SCRIPT_DIR /w/hallb-scshelf2102/clas12/asportes/2N-Analyser/setup/scripts/screen
# setenv SCREEN_SCRIPT_DIR ./setup/scripts/screen

chmod +x $SCREEN_SCRIPT_DIR/start_screen_session.csh
# chmod +x ./setup/scripts/screen/start_screen_session.csh
# source ./setup/scripts/screen/start_screen_session.csh

echo "${COLOR_START}Screen commands have been updated.${COLOR_END}"
echo "${COLOR_START}To run a screen session, use the following command:${COLOR_END}"
echo '$SCREEN_SCRIPT_DIR/start_screen_session.csh <session_name> <command>'
echo ""