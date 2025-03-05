#!/bin/tcsh

unset SCREEN_SCRIPT_DIR
setenv SCREEN_SCRIPT_DIR /w/hallb-scshelf2102/clas12/asportes/2N-Analyser/setup/scripts/screen
# setenv SCREEN_SCRIPT_DIR ./setup/scripts/screen

chmod +x $SCREEN_SCRIPT_DIR/start_screen_session.csh
# chmod +x ./setup/scripts/screen/start_screen_session.csh
# source ./setup/scripts/screen/start_screen_session.csh

echo ""
echo "Screen commands have been updated."
echo "to run a screen session, use the following command:"
echo "./$SCREEN_SCRIPT_DIR/start_screen_session.csh <session_name> <command>"
echo ""