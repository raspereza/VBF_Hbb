#!/bin/sh
#combineCards.py datacards_doubleb_CAT0.txt datacards_doubleb_CAT1.txt datacards_doubleb_CAT2.txt datacards_doubleb_CAT3.txt > datacards_doubleb.txt
#cp SingleBTag/signal_singleb_shapes.root ./
#cp SingleBTag/data_singleb_shapes.root ./
rm datacards_singleb_${1}.txt
cp SingleBTag/datacard/datacards_singleb_CAT*_${1}.txt ./
combineCards.py datacards_singleb_CAT*_${1}.txt > datacards_singleb_${1}.txt
#combineCards.py datacards_doubleb.txt datacards_singleb.txt > datacards_all.txt
