#!/bin/bash
# $1 - datacard name w/o extension *.txt 
# $2 - signal strength
n=$#
if [[ $n -ne 2 ]]; then
    echo usage : Significance_Asimov.bash [DATACARD_NAME] [SIGNAL_STRENGHT]
    echo DATACARD_NAME = [datacards_Run2, datacards_Run3, datacards_Run2_Run3]
    echo SIGNAL_STRENGHT = signal strength
    exit
fi

datacard=$1 
mu=$2
folder=datacards_Run2_Run3
cd ${folder}
#combine -M Significance --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --X-rtd ADDNLL_RECURSIVE=0 --X-rtd FITTER_NEW_CROSSING_ALGO -t -1 --expectSignal=${mu} -m 125 ${datacard}.txt
combineTool.py -M Significance --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --expectSignal=${mu} -t -1 --rMin=-3 --rMax=3 -m 125 ${1}.txt
cd -
