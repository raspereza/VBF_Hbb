root -l -q -b CreateDataTemplates_exp_multipdf.C
root -l -q -b CreateSignalTemplates_sys_full_2.C
root -l -q -b CreateZJetscombinedTemplates_sys_full_2.C
root -l -q -b createdatacards_mult.C
combineCards.py qqh_1=datacard_cat0mult.txt  qqh_2=datacard_cat1mult.txt  qqh_3=datacard_cat2mult.txt ggh_1=datacard_cat3mult.txt ggh_2=datacard_cat4mult.txt zj_1=datacard_cat5mult.txt zj_2=datacard_cat6mult.txt > datacards_qqH_2023mult.txt
combineCards.py cat0=datacard_cat0mult.txt  cat1=datacard_cat1mult.txt  cat2=datacard_cat2mult.txt > datacardmult.txt
{
./Significance_Asimov.bash datacardmult 1
./Significance_Asimov.bash datacards_qqH_2023mult 1
./Significance_Asimov.bash datacard_cat0mult 1
./Significance_Asimov.bash datacard_cat1mult 1
./Significance_Asimov.bash datacard_cat2mult 1
./Significance_Asimov.bash datacard_cat3mult 1
./Significance_Asimov.bash datacard_cat4mult 1
./Significance_Asimov.bash datacard_cat5mult 1
./Significance_Asimov.bash datacard_cat6mult 1
} | tee output.txt

text2workspace.py datacards_qqH_2023mult.txt -o datacards.root
combine -M MultiDimFit datacards.root --algo grid -P r --points 100 --setParameterRanges r=-5,5 --expectSignal 1 -t -1 --robustFit 1 --cminDefaultMinimizerStrategy 0 --cminFallbackAlgo Minuit2,Migrad,0:0.1 -v 3
python3 /eos/home-s/sapradha/PhD_Projects/VBF_analysis/Statistical/CMSSW_14_1_0_pre4/bin/el9_amd64_gcc12/plot1DScan.py         higgsCombineTest.MultiDimFit.mH120.root         --main-label " qqH r scan "         --POI r         --output "${LOCALFILE}"         --main-color 4 
cp c1.pdf /eos/home-s/sapradha/www/VBFHbb_Run3/NeuralNetwork/DNN_2023/
cp c1.png /eos/home-s/sapradha/www/VBFHbb_Run3/NeuralNetwork/DNN_2023/

text2workspace.py datacards_qqH_2023mult.txt -o datacard.root
mkdir impact
mv datacard.root impact
cd impact
combineTool.py -M Impacts -d datacard.root -m 125 --doInitialFit --robustFit 1  --cminDefaultMinimizerStrategy 0 --cminFallbackAlgo Minuit2,Migrad,0:0.1  --X-rtd TMCSO_AdaptivePseudoAsimov=0 --X-rtd TMCSO_PseudoAsimov=0 --X-rt MINIMIZER_freezeDisassociatedParams --X-rtd MINIMIZER_multiMin_hideConstants --X-rtd MINIMIZER_multiMin_maskConstraints --X-rtd MINIMIZER_multiMin_maskChannels=2   -n _impact_vbfhbb --redefineSignalPOIs r --rMax 5.0 --rMin -5.0 --expectSignal 1 -t -1
combineTool.py -M Impacts -d datacard.root -m 125 --doFits --robustFit 1  --cminDefaultMinimizerStrategy 0 --cminFallbackAlgo Minuit2,Migrad,0:0.1  --X-rtd TMCSO_AdaptivePseudoAsimov=0 --X-rtd TMCSO_PseudoAsimov=0 --X-rt MINIMIZER_freezeDisassociatedParams --X-rtd MINIMIZER_multiMin_hideConstants --X-rtd MINIMIZER_multiMin_maskConstraints --X-rtd MINIMIZER_multiMin_maskChannels=2   -n _impact_vbfhbb --redefineSignalPOIs  r --rMax 5.0 --rMin -5.0 --expectSignal 1 -t -1
combineTool.py -M Impacts -d datacard.root -m 125 -o impact_vbfhbb.json -n _impact_vbfhbb --redefineSignalPOIs r --rMax 5.0 --rMin -5.0
plotImpacts.py -i impact_vbfhbb.json -o impact_discrete_profile
cp impact_discrete_profile.pdf /eos/home-s/sapradha/www/VBFHbb_Run3/NeuralNetwork/DNN_2023/
