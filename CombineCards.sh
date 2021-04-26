#!/bin/sh
# $1 = with_sys, bias

rm datacards_singleb_${1}.txt
cp SingleBTag/datacards_${1}/datacards_singleb_CAT*.txt ./
combineCards.py datacards_singleb_CAT*.txt > datacards_singleb_${1}.txt

rm datacards_doubleb_${1}.txt
cp DoubleBTag/datacards_${1}/datacards_doubleb_CAT*.txt ./
combineCards.py datacards_doubleb_CAT*.txt > datacards_doubleb_${1}.txt

rm datacards_${1}.txt
combineCards.py datacards_singleb_CAT*.txt datacards_doubleb_CAT*.txt > datacards_${1}.txt
