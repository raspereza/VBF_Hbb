#!/bin/bash
# $1 - datacards w/o root extension
# $2 - VBF signal strength
combine -M FitDiagnostics --robustFit 1 --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --setParameters r_qqH=${2},r_ggH=1 --redefineSignalPOIs r_qqH -t -1 -m 125 -d ${1}.root -v 5 
