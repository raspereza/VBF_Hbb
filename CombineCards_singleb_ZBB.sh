#!/bin/sh
rm -rf datacards_singleb_ZBB.txt datacards_singleb_CAT*ZBB.txt
cp ZBB_SingleB/datacards/datacards_singleb_CAT*ZBB.txt ./
combineCards.py datacards_singleb_CAT*ZBB.txt > datacards_singleb_ZBB.txt
