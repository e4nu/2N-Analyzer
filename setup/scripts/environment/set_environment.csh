#!/bin/tcsh

echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Updating environment                                                =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo ""

unset COLOR_START
setenv COLOR_START '\033[35m'

unset COLOR_END
setenv COLOR_END '\033[0m'

echo "${COLOR_START}- Updating environment ------------------------------------------------${COLOR_END}"
echo ""

unset DIR_2N_ANALYSIS_CODE
# setenv DIR_2N_ANALYSIS_CODE $(pwd)
setenv DIR_2N_ANALYSIS_CODE `pwd`
# setenv DIR_2N_ANALYSIS_CODE /w/hallb-scshelf2102/clas12/asportes/2N-Analyser
echo "${COLOR_START}DIR_2N_ANALYSIS_CODE:${COLOR_END} ${DIR_2N_ANALYSIS_CODE}"
echo ""
