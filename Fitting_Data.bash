#!/bin/bash
# $1 - datacards
combine -M FitDiagnostics  --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --X-rtd ADDNLL_RECURSIVE=0 --X-rtd FITTER_NEW_CROSSING_ALGO --robustFit 1 --rMin=-30 --rMax=30 -m 125 ${1}.txt --cminDefaultMinimizerTolerance 0.01 --cminDefaultMinimizerStrategy=0 -v 5
#--saveNormalizations --saveShapes --saveWithUncertainties --saveNLL -v2 
#combine -M FitDiagnostics --robustHesse 1 --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --rMin=-20 --rMax=20 -m 125 ${1}.txt -v 5 
# --saveNormalizations --saveShapes --saveWithUncertainties --saveNLL
#combine -M FitDiagnostics --robustFit 1 --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --rMin=-10 --rMax=10 -m 125 ${1}.txt -v 5
mv fitDiagnosticsTest.root ${1}_data.root
