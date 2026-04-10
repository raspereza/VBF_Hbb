#!/bin/bash
# $1 - suffix
# $2 - signal strength
datacard=$1
mu=$2
combine -M Significance --cminDefaultMinimizerTolerance 0.01 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy=0 --X-rtd FITTER_NEW_CROSSING_ALGO --expectSignal=${mu} --rMin=-3 --rMax=3 -m 125 ${1}.txt

