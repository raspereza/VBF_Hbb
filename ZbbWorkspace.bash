#!/bin/bash
text2workspace.py ${1}.txt -m 125.0 -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO 'map=.*/zj:r[1,-5,10]' -o ${1}.root
