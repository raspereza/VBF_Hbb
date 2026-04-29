#!/bin/bash
# $1 - datacards w/o root extension
# $2 - VBF signal strength
n=$#
if [[ $n -ne 2 ]]; then
    echo usage : SignificanceVBF_Asimov.bash [DATACARD_NAME] [SIGNAL_STRENGHT]
    echo DATACARD_NAME = [datacards_Run2_2POIs, datacards_Run3_2POIs, datacards_Run2_Run3_2POIs]
    echo SIGNAL_STRENGHT = signal strength of qqH
    exit
fi
cd datacards_Run2_Run3
combineTool.py -M Significance --setParameters r_ggH=1,r_qqH=${2} --redefineSignalPOIs r_qqH --freezeParameters r_ggH --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO -t -1 --rMin=-10 --rMax=10 -m 125 -d ${1}.root
cd -

