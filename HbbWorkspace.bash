#!/bin/bash
text2workspace.py ${1}.txt -m 125.0 -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO 'map=.*/qqH_hbb:r_qqH[1,-5,10]' --PO 'map=.*/ggH_hbb:r_ggH[1,-5,10]' -o ${1}.root
