#!/bin/sh

rm datacards_doubleb_2018_UL.txt
cp DoubleBTag_2018_UL/datacards/datacards_doubleb_DoubleB*_2018.txt ./
combineCards.py datacards_doubleb_DoubleB*_2018.txt > datacards_doubleb_2018_UL.txt
