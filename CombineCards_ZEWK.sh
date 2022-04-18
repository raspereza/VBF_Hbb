#!/bin/sh

rm *.txt
# double-b 2016
cp DoubleBTag_2016_UL_EWK/datacards/datacards_doubleb_DoubleB*_2016_EWK.txt ./
combineCards.py datacards_doubleb_DoubleB*_2016_EWK.txt > datacards_doubleb_2016_EWK.txt
# double-b 2018
cp DoubleBTag_2018_UL_EWK/datacards/datacards_doubleb_DoubleB*_2018_EWK.txt ./
combineCards.py datacards_doubleb_DoubleB*_2018_EWK.txt > datacards_doubleb_2018_EWK.txt
# single-b 2016
cp SingleBTag_2016_UL_EWK/datacards/datacards_singleb_CAT*_2016_EWK.txt ./
combineCards.py datacards_singleb_CAT*_2016_EWK.txt > datacards_singleb_2016_EWK.txt
# single-b 2018
cp SingleBTag_2018_UL_EWK/datacards/datacards_singleb_CAT*_2018_EWK.txt ./
combineCards.py datacards_singleb_CAT*_2018_EWK.txt > datacards_singleb_2018_EWK.txt
# double-b Run2
combineCards.py datacards_doubleb_2016_EWK.txt datacards_doubleb_2018_EWK.txt > datacards_doubleb_Run2_EWK.txt
# single-b Run2
combineCards.py datacards_singleb_2016_EWK.txt datacards_singleb_2018_EWK.txt > datacards_singleb_Run2_EWK.txt
# Run 2
combineCards.py datacards_doubleb_2016_EWK.txt datacards_doubleb_2018_EWK.txt datacards_singleb_2016_EWK.txt datacards_singleb_2018_EWK.txt > datacards_Run2_EWK.txt



