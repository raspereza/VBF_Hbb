#!/bin/bash
# $1 - datacards w/o root extension
combine -M FitDiagnostics --robustFit 1 --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --redefineSignalPOIs r_qqH,r_ggH -m 125 -d ${1}.root -v 3 
#combine -M FitDiagnostics --robustFit 1 --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --redefineSignalPOIs r_qqH,r_ggH -m 125 -d ${1}.root -v 3
mv fitDiagnosticsTest.root ${1}_multiDim_data.root
