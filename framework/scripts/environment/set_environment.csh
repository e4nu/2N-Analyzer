#!/bin/tcsh

# This script sets up the environment variables for the 2N-Analyzer code. It updates the environment by setting the DIR_2N_ANALYSIS_CODE variable to the current working directory, and it also sets the ANALYSIS_HOSTNAME variable to the hostname of the machine. Additionally, it checks if the hostname contains a specific substring (defined in JLAB_TESTER) and sets the IFARM_RUN variable accordingly. The script also uses color codes to make the output more visually appealing.
# Note: The color codes are defined using ANSI escape sequences, and they are used to highlight the output in the terminal. The DIR_2N_ANALYSIS_CODE variable is set to the current working directory, which allows the code to reference files and directories relative to the location of the script. The ANALYSIS_HOSTNAME variable is set to the hostname of the machine, which can be useful for determining whether certain commands should be run based on the environment. The IFARM_RUN variable is set based on whether the hostname contains a specific substring, which allows for conditional execution of commands based on the environment.

echo "${COLOR_START}===========================================================================================${COLOR_END}"
echo "${COLOR_START}= Updating environment                                                                    =${COLOR_END}"
echo "${COLOR_START}===========================================================================================${COLOR_END}"
echo ""

unset COLOR_START
setenv COLOR_START '\033[35m'

unset COLOR_END
setenv COLOR_END '\033[0m'

echo "${COLOR_START}- Updating environment --------------------------------------------------------------------${COLOR_END}"
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
