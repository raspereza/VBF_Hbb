#!/bin/sh

rm datacards_doubleb_2016_UL.txt
cp DoubleBTag_2016_UL/datacards/datacards_doubleb_DoubleB*_2016.txt ./
combineCards.py datacards_doubleb_DoubleB*_2016.txt > datacards_doubleb_2016_UL.txt
