root -l -q -b CreateDataTemplates_exp.C
root -l -q -b CreateSignalTemplates_sys_full_2.C
root -l -q -b CreateZJetscombinedTemplates_sys_full_2.C
root -l -q -b createdatacards_bias.C
combineCards.py qqh_2022_cat1=datacard_cat0.txt  qqh_2022_cat2=datacard_cat1.txt  qqh_2022_cat3=datacard_cat2.txt  ggh_2022_cat1=datacard_cat3.txt  ggh_2022_cat2=datacard_cat4.txt  zj_2022_cat1=datacard_cat5.txt  zj_2022_cat2=datacard_cat6.txt > datacard_2022.txt
./Significance_Asimov.bash datacard_2022 1
