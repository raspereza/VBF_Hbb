#!/bin/bash
# $1 - directory to store datacards 
rm *.txt
dir=$1
rm -rf workspace
rm -rf ${dir}
mkdir workspace
mkdir ${dir}
# double-b 2016
cp DoubleBTag_2016_UL_Preapproval_kNLO_v2/datacards/datacards_doubleb_DoubleB*_2016_Preapproval.txt ./
combineCards.py datacards_doubleb_DoubleB*_2016_Preapproval.txt > datacards_doubleb_2016.txt
cp DoubleBTag_2016_UL_Preapproval_kNLO_v2/root_shape/*.root workspace/
# double-b 2018
cp DoubleBTag_2018_UL_Preapproval_kNLO_v2/datacards/datacards_doubleb_DoubleB*_2018_Preapproval.txt ./
combineCards.py datacards_doubleb_DoubleB*_2018_Preapproval.txt > datacards_doubleb_2018.txt
cp DoubleBTag_2018_UL_Preapproval_kNLO_v2/root_shape/*.root workspace/
# single-b 2016
cp SingleBTag_2016_UL_Preapproval_kNLO_v2/datacards/datacards_singleb_SingleB*_2016_Preapproval.txt ./
combineCards.py datacards_singleb_SingleB*_2016_Preapproval.txt > datacards_singleb_2016.txt
cp SingleBTag_2016_UL_Preapproval_kNLO_v2/root_shape/*.root workspace/
# single-b 2018
cp SingleBTag_2018_UL_Preapproval_kNLO_v2/datacards/datacards_singleb_SingleB*_2018_Preapproval.txt ./
combineCards.py datacards_singleb_SingleB*_2018_Preapproval.txt > datacards_singleb_2018.txt
cp SingleBTag_2018_UL_Preapproval_kNLO_v2/root_shape/*.root workspace/
# double-b Run2
combineCards.py datacards_doubleb_2016.txt datacards_doubleb_2018.txt > datacards_doubleb_Run2.txt
# single-b Run2
combineCards.py datacards_singleb_2016.txt datacards_singleb_2018.txt > datacards_singleb_Run2.txt
# Run 2
combineCards.py datacards_doubleb_2016.txt datacards_doubleb_2018.txt datacards_singleb_2016.txt datacards_singleb_2018.txt > datacards_Run2.txt
mv *.txt ${dir}
mv workspace ${dir}


