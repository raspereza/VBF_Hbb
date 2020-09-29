#!/bin/bash
# $1 - suffix
# $2 - signal strength
cat=$1 
combine -M ProfileLikelihood  --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --X-rtd ADDNLL_RECURSIVE=0 --X-rtd FITTER_NEW_CROSSING_ALGO -m 125 datacards_${cat}.txt
