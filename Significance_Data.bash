#!/bin/bash
# $1 - datacard name w/o extension *.txt
n=$#
if [[ $n -ne 1 ]]; then
    echo usage : Significance_Data.bash [DATACARD_NAME]
    echo DATACARD_NAME = [datacards_Run2, datacards_Run3, datacards_Run2_Run3]
    exit
fi

datacards=$1
cd datacards_Run2_Run3
combine -M Significance --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --X-rtd ADDNLL_RECURSIVE=0 --X-rtd FITTER_NEW_CROSSING_ALGO --expectSignal=0 -m 125 ${datacards}.txt
cd -
