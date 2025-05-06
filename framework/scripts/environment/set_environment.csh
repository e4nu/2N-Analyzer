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
# setenv DIR_2N_ANALYSIS_CODE /w/hallb-scshelf2102/clas12/asportes/2N-Analyzer
echo "${COLOR_START}DIR_2N_ANALYSIS_CODE:${COLOR_END} ${DIR_2N_ANALYSIS_CODE}"
echo ""

unset ANALYSIS_HOSTNAME
# setenv DIR_2N_ANALYSIS_CODE $(pwd)
setenv ANALYSIS_HOSTNAME `hostname`
# setenv DIR_2N_ANALYSIS_CODE /w/hallb-scshelf2102/clas12/asportes/2N-Analyzer
echo "${COLOR_START}ANALYSIS_HOSTNAME:${COLOR_END} ${ANALYSIS_HOSTNAME}"
echo ""

unset JLAB_TESTER
# setenv DIR_2N_ANALYSIS_CODE $(pwd)
setenv JLAB_TESTER "jlab.org"
# setenv DIR_2N_ANALYSIS_CODE /w/hallb-scshelf2102/clas12/asportes/2N-Analyzer
echo "${COLOR_START}JLAB_TESTER:${COLOR_END} ${JLAB_TESTER}"
# echo ""

unset IFARM_RUN

# Check if the hostname contains the substring
if ( "$ANALYSIS_HOSTNAME" =~ *"$JLAB_TESTER"* ) then
    echo "${COLOR_START}The hostname contains '$JLAB_TESTER'. Running the commands for this case.${COLOR_END}"
    # echo ""
    # Put the commands to run if the hostname contains the substring here
    setenv IFARM_RUN 1
else
    echo "${COLOR_START}The hostname does not contain '$JLAB_TESTER'. Running the alternate commands.${COLOR_END}"
    # echo ""
    # Put the commands to run if the hostname does not contain the substring here
    setenv IFARM_RUN 0
endif

echo "${COLOR_START}IFARM_RUN:${COLOR_END} ${IFARM_RUN}"
echo ""
