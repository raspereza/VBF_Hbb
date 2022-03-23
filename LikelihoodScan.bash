#!/bin/bash
#ulimit -s unlimited
# 1 : datacards
# 2 : POI
# 3 : POI min
# 4 : POI max 
combineTool.py -m 125 -M MultiDimFit --setParameters r_qqH=1,r_ggH=1,normZ=1 --setParameterRanges ${2}=${3},${4} --points 100 --redefineSignalPOIs ${2} -d ${1}.root --algo grid -n .${2} --alignEdges 1 --cminDefaultMinimizerStrategy=0  -t -1



