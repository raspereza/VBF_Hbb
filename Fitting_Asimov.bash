#!/bin/bash
# $1 - suffix
# $2 - signal strength
datacard=$1 
mu=$2
combine -M FitDiagnostics  --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --X-rtd ADDNLL_RECURSIVE=0 --X-rtd FITTER_NEW_CROSSING_ALGO -t -1 --expectSignal=${mu} --robustFit 1 --rMin=-20 --rMax=20 -m 125 ${datacard}.txt --cminDefaultMinimizerTolerance 0.01 --cminDefaultMinimizerStrategy=0 --saveNormalizations --saveShapes --saveWithUncertainties --saveNLL -v2
mv fitDiagnostics.root mlfit_${cat}_r${mu}.root
mv higgsCombineTest.FitDiagnostics.mH125.root limit_${cat}_r${mu}.root
cd ../../
