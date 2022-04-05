#!/bin/sh

rm *.txt
./CombineCards_DoubleB_2016_UL.sh
./CombineCards_DoubleB_2018_UL.sh
./CombineCards_SingleB_2016_UL_v2.sh
./CombineCards_SingleB_2018_UL_v2.sh
combineCards.py datacards_doubleb_2016_UL.txt datacards_doubleb_2018_UL.txt datacards_singleb_2016_UL_v2.txt datacards_singleb_2018_UL_v2.txt > datacards_Run2_UL.txt

