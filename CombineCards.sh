#!/bin/sh
#combineCards.py datacards_doubleb_CAT0.txt datacards_doubleb_CAT1.txt datacards_doubleb_CAT2.txt datacards_doubleb_CAT3.txt > datacards_doubleb.txt
cp SingleBTag/signal_singleb_shapes.root ./
cp SingleBTag/data_singleb_shapes.root ./
cp SingleBTag/datacards_singleb_*.txt ./
combineCards.py datacards_singleb_CAT4.txt datacards_singleb_CAT5.txt datacards_singleb_CAT6.txt datacards_singleb_CAT7.txt datacards_singleb_CAT8.txt > datacards_singleb.txt
#combineCards.py datacards_doubleb.txt datacards_singleb.txt > datacards_all.txt