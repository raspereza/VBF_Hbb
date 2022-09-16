#!/bin/bash
text2workspace.py ${1}.txt -m 125.0 -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO 'map=.*/qqH_hbb:r_Hbb[1,-10,10]' --PO 'map=.*/ggH_hbb:r_Hbb[1,-20,20]' -o ${1}.root
