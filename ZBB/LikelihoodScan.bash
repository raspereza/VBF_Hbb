#!/bin/bash
#ulimit -s unlimited
#combineTool.py -m 91 -M MultiDimFit --setParameters r=1 --setParameterRanges r=0.8,1.8 --points 51 --redefineSignalPOIs r -d $1.root --algo grid -n .r_obs --alignEdges 1 --cminDefaultMinimizerStrategy=0 

python scripts/plot1DScan.py --main=higgsCombine.r_obs.MultiDimFit.mH91.root --POI=r --output=r --no-numbers --no-box --x_title="#mu_{Zbb}" --x-min=0.6 --x-max=2.0 --y-max=2.0 --main-label="#mu_{Zbb} "

