#!/bin/sh

rm datacards_singleb_2016_UL_v3.txt
cp SingleBTag_2016_UL_v3/datacards/datacards_singleb_CAT*_2016_v3.txt ./
combineCards.py datacards_singleb_CAT*_2016_v3.txt > datacards_singleb_2016_UL_v3.txt
