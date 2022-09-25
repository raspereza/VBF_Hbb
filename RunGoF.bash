#!/bin/bash
# $1 - datacards w/o txt extension
# $2 - algo (saturated, KS, AD)
# $3 - number of toys (typically >1000)
ulimit -s unlimited
ws=$1
algo=$2
ntoys=$3
combine -M GoodnessOfFit -d ${ws}.txt -m 125 --algo ${algo} -n _obs.${ws}
combine -M GoodnessOfFit -d ${ws}.txt -m 125 -t ${ntoys} --toysFreq --algo ${algo} -n _exp.${ws}
