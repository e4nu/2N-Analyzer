#!/bin/tcsh

source update_only.sh

echo "\033[35m- Launching data-analysis code -------------------------------------\033[0m"
echo ""
clas12root -b -q ./main.c
# if ( "$IFARM_RUN" == "1" ) then
#     echo "clas12root -b -q -DIFARM_RUN=$IFARM_RUN ./main.c"
#     # If IFARM_RUN is 1, run the macro with WORK_DIR set
#     # clas12root -DWORK_DIR=\"$(pwd)\" -b -q ./main.c
#     # clas12root -DWORK_DIR="`pwd`" -b -q ./main.c
#     clas12root -b -q -DIFARM_RUN=$IFARM_RUN ./main.c
# else
#     # If IFARM_RUN is not 1, just run the macro without WORK_DIR
#     clas12root -b -q ./main.c
# endif
echo ""
echo "\033[35m- Operation finished --------------------------------------------------\033[0m"
echo ""
