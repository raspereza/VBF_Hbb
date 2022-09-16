#!/bin/bash
#ulimit -s unlimited
# 1 : datacards
# 2 : POI
# 3 : POI min
# 4 : POI max 

plot1DScan.py --main=higgsCombine.${1}.MultiDimFit.mH125.root --POI=r --output=scan_${1} --no-numbers --no-box --x_title="r_{qqH}" --x-min=-1.0 --x-max=3.0 --y-max=2.0 --main-label=""

