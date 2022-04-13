#!/bin/sh

rm datacards_doubleb_2018_UL_v2.txt
cp DoubleBTag_2018_UL_v2/datacards/datacards_doubleb_DoubleB*_2018.txt ./
combineCards.py datacards_doubleb_DoubleB*_2018.txt > datacards_doubleb_2018_UL_v2.txt
