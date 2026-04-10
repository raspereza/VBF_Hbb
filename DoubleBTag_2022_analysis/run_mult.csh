root -l -q -b CreateDataTemplates_exp_multipdf.C
root -l -q -b CreateSignalTemplates_sys_full_2.C
root -l -q -b CreateZJetscombinedTemplates_sys_full_2.C
root -l -q -b createdatacards_mult.C
combineCards.py qqh_1=datacard_cat0mult.txt  qqh_2=datacard_cat1mult.txt  qqh_3=datacard_cat2mult.txt ggh_1=datacard_cat3mult.txt ggh_2=datacard_cat4mult.txt zj_1=datacard_cat5mult.txt zj_2=datacard_cat6mult.txt > datacards_qqH_2022mult.txt
#combineCards.py cat0=datacard_cat0mult.txt  cat1=datacard_cat1mult.txt  cat2=datacard_cat2mult.txt > datacardmult.txt
./Significance_Asimov.bash datacards_qqH_2022mult 1
./Significance_Data.bash datacards_qqH_2022mult 1

