#!/bin/tcsh

# ------------------------------------------------------------------------------------------
# setup_screen_commands.csh
# ------------------------------------------------------------------------------------------
# Purpose
# -------
# Initializes the directory paths and aliases used for managing screen
# sessions within the 2N Analyzer environment.
# ------------------------------------------------------------------------------------------

# Header banner

echo "${COLOR_START}====================================================================================================${COLOR_END}"
echo "${COLOR_START}= Setting up screen functions                                                                      =${COLOR_END}"
echo "${COLOR_START}====================================================================================================${COLOR_END}"
echo ""

# ------------------------------------------------------------------------------------------
# Define location of screen scripts
# ------------------------------------------------------------------------------------------

echo "${COLOR_START}- Setting screen directory -------------------------------------------------------------------------${COLOR_END}"
echo ""

# Reset variable
unset SCREEN_SCRIPT_DIR

# Set path to screen script directory relative to project root
setenv SCREEN_SCRIPT_DIR ${DIR_2N_ANALYSIS_CODE}/scripts/screen

echo "${COLOR_START}SCREEN_SCRIPT_DIR:${COLOR_END} ${SCREEN_SCRIPT_DIR}"
echo ""

# ------------------------------------------------------------------------------------------
# Load screen aliases
# ------------------------------------------------------------------------------------------

source $SCREEN_SCRIPT_DIR/setup_screen_aliases.csh

# ------------------------------------------------------------------------------------------
# Ensure start_screen_session script is executable
# ------------------------------------------------------------------------------------------

echo "${COLOR_START}- Setting screen command --------------------------------------------------------------------------${COLOR_END}"
echo ""

chmod +x $SCREEN_SCRIPT_DIR/start_screen_session.csh

echo "${COLOR_START}Screen commands have been updated.${COLOR_END}"
echo "${COLOR_START}To run a screen session, use the following command:${COLOR_END}"
echo '$SCREEN_SCRIPT_DIR/start_screen_session.csh <session_name> <command> [stay_open]'
echo ""