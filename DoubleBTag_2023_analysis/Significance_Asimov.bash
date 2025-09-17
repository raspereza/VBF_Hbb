#!/bin/bash
# $1 - suffix
# $2 - signal strength
datacard=$1
mu=$2
#combine -M Significance --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --X-rtd ADDNLL_RECURSIVE=0 --X-rtd FITTER_NEW_CROSSING_ALGO -t -1 --expectSignal=${mu} -m 125 ${datacard}.txt
combine -M Significance --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --expectSignal=${mu} -t -1 --rMin=-3 --rMax=3 -m 125 ${1}.txt --toysFrequentist
combine -M Significance --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --expectSignal=${mu} -t -1 --rMin=-3 --rMax=3 -m 125 ${1}.txt
#combine -M Significance --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --rMin=-3 --rMax=3 -m 125 ${1}.txt
