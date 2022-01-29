#!/bin/sh

rm datacards_singleb_2016_UL_v2.txt
cp SingleBTag_2016_UL_v2/datacards/datacards_singleb_CAT*_2016_v2.txt ./
combineCards.py datacards_singleb_CAT*_2016_v2.txt > datacards_singleb_2016_UL_v2.txt
