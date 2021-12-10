#!/bin/bash
# $1 - suffix
datacard=$1 
#combine -M FitDiagnostics  --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --X-rtd ADDNLL_RECURSIVE=0 --X-rtd FITTER_NEW_CROSSING_ALGO --expectSignal=1 --robustFit 1 --rMin=-20 --rMax=20 -m 125 ${datacard}.txt --cminDefaultMinimizerTolerance 0.01 --cminDefaultMinimizerStrategy=0 --saveNormalizations --saveShapes --saveWithUncertainties --saveNLL -v2 
combine -M FitDiagnostics --robustHesse 1 --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --rMin=-20 --rMax=20 -m 125 ${1}.txt -v 5 
