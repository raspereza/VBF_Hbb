#!/bin/bash
cd datacards_Run2_Run3
n=$#
if [[ $n -ne 1 ]]; then
    echo usage :  HbbWorkspace.bash [DATACARD_NAME] [SIGNAL_STRENGHT]
    echo DATACARD_NAME = [datacards_Run2, datacards_Run3, datacards_Run2_Run3]
    exit
fi
text2workspace.py ${1}.txt -m 125.0 -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO 'map=.*/qqH_hbb:r_qqH[1,-30,30]' --PO 'map=.*/ggH_hbb:r_ggH[1,-30,30]' -o ${1}_2POIs.root
cd -
