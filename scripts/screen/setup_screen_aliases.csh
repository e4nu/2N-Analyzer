#!/bin/tcsh

# ------------------------------------------------------------------------------------------
# setup_screen_aliases.csh
# ------------------------------------------------------------------------------------------
# Purpose
# -------
# Defines convenient shell aliases for managing GNU Screen sessions used by
# the 2N Analyzer workflow.
#
# The aliases allow quick control over analysis jobs running in detached
# screen sessions.
# ------------------------------------------------------------------------------------------

echo "${COLOR_START}- Setting screen aliases ---------------------------------------------------------------------------${COLOR_END}"
echo ""

# Reload this alias script
alias setup_screen_aliases "source ${DIR_2N_ANALYSIS_CODE}/scripts/screen/setup_screen_aliases.csh"
echo "${COLOR_START}setup_screen_aliases:${COLOR_END} `alias setup_screen_aliases`"

# Kill all detached screen sessions
alias kill_all1 "screen -ls | grep Detached | cut -d. -f1 | awk '{print \$1}' | xargs kill"
echo "${COLOR_START}kill_all1:${COLOR_END} `alias kill_all1`"

# Kill all screen sessions safely
alias kill_all2 "screen -ls | grep -E '^\\s*[0-9]+' | awk '{print \$1}' | cut -d. -f1 | xargs -r -I {} screen -S {} -X quit"
echo "${COLOR_START}kill_all2:${COLOR_END} `alias kill_all2`"

# Convenience alias to run main analysis
alias crunc "$SCREEN_SCRIPT_DIR/start_screen_session.csh '' 'source run.sh'"
echo "${COLOR_START}crunc:${COLOR_END} `alias crunc`"

# Convenience alias to run neutron efficiency tester
alias srunnfd "$SCREEN_SCRIPT_DIR/start_screen_session.csh '' 'source nFD_eff_tester_run.sh'"
echo "${COLOR_START}srunnfd:${COLOR_END} `alias srunnfd`"

echo ""
echo "${COLOR_START}Screen aliases have been set.${COLOR_END}"

echo "${COLOR_START}To reset these aliases:${COLOR_END}"
echo 'source $SCREEN_SCRIPT_DIR/setup_screen_aliases.csh'
echo ""