#!/bin/bash
text2workspace.py ${1}.txt -m 125.0 -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO 'map=.*/qqH_hbb:r_qqH[1,-20,20]' --PO 'map=.*/ggH_hbb:r_ggH[1,-30,30]' -o ${1}.root
