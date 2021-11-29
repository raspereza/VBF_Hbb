#!/bin/bash
# $1 - suffix
# $2 - signal strength
datacards=$1 
#combine -M Significance --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --X-rtd ADDNLL_RECURSIVE=0 --X-rtd FITTER_NEW_CROSSING_ALGO --expectSignal=0 -m 125 ${datacards}.txt
combineTool.py -M Significance --expectSignal=0 --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 -m 125 ${1}.txt
