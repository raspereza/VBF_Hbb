#!/bin/bash
# $1 - Category
cat=$1 
combine -M FitDiagnostics  --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --X-rtd ADDNLL_RECURSIVE=0 --X-rtd FITTER_NEW_CROSSING_ALGO --robustFit 1 --rMin=-100 --rMax=100 -m 125 datacards_${cat}.txt --cminDefaultMinimizerTolerance 0.01 --saveNormalizations --saveShapes --saveWithUncertainties --saveNLL -v2
mv fitDiagnostics.root mlfit_${cat}_obs.root
mv higgsCombineTest.FitDiagnostics.mH125.root limit_${cat}_obs.root
cd ../../
