#!/bin/bash
# $1 - RooT file with workspace (w/o extension root)
datacard=$1 
combine -M FitDiagnostics  --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --X-rtd ADDNLL_RECURSIVE=0 --setParameters r=1 --redefineSignalPOIs r --X-rtd FITTER_NEW_CROSSING_ALGO --robustFit 1 --rMin=-20 --rMax=20 -m 125 -d ${datacard}.root --cminDefaultMinimizerTolerance 0.01 --cminDefaultMinimizerStrategy=0 --saveNormalizations --saveShapes --saveWithUncertainties --saveNLL -v2 
mv fitDiagnostics.root mlfit_obs.root
