#!/bin/bash
# $1 - datacard w/o root extension
n=$#
if [[ $n -ne 1 ]]; then
    echo usage : Significance_Asimov.bash [DATACARD_NAME] 
    echo DATACARD_NAME = [datacards_Run2_2POIs, datacards_Run3_2POIs, datacards_Run2_Run3_2POIs]
    exit
fi
cd datacards_Run2_Run3
combineTool.py -M Significance --setParameters r_ggH=1,r_qqH=1 --freezeParameters r_ggH --redefineSignalPOIs r_qqH --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --rMin=-10 --rMax=10 -m 125 ${1}.root
cd -
