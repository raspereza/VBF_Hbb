#!/bin/bash
# $1 - datacards w/o root extension
# $2 - VBF signal strength
combineTool.py -M Significance --setParameters r_ggH=1,r_qqH=${2} --redefineSignalPOIs r_qqH --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO -t -1 --rMin=-10 --rMax=10 -m 125 -d ${1}.root
