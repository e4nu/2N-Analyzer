#!/bin/tcsh

echo "${COLOR_START}- Setting screen aliases ----------------------------------------------${COLOR_END}"
echo ""

alias setup_screen_aliases "source ${DIR_2N_ANALYSIS_CODE}/setup/scripts/screen/setup_screen_aliases.csh"
echo "${COLOR_START}setup_screen_aliases:${COLOR_END} `alias setup_screen_aliases`"
alias kill_all1 "screen -ls | grep Detached | cut -d. -f1 | awk '{print \$1}' | xargs kill"
echo "${COLOR_START}kill_all1:${COLOR_END} `alias kill_all1`"
alias kill_all2 "screen -ls | grep -E '^\s*[0-9]+' | awk '{print \$1}' | cut -d. -f1 | xargs -r -I {} screen -S {} -X quit"
echo "${COLOR_START}kill_all2:${COLOR_END} `alias kill_all2`"
echo ""
echo "${COLOR_START}Screen aliases have been set.${COLOR_END}"
echo "${COLOR_START}To reset these aliases:${COLOR_END}"
echo 'source $SCREEN_SCRIPT_DIR/setup_screen_aliases.csh'
echo ""