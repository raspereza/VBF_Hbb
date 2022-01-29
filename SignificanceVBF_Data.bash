#!/bin/bash
# $1 - datacard w/o root extension
combineTool.py -M Significance --setParameters r_ggH=1,r_qqH=1 --redefineSignalPOIs r_qqH --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --rMin=-10 --rMax=10 -m 125 ${1}.root
