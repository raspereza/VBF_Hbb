#include "CreateMbbBdtNtuple.C"
void CreateMbbBdtNtupleSeq()
{
  // Samples
  std::vector<TString> Samples;
  Samples.push_back("Full_Data");
  Samples.push_back("GluGluHToBB_M125");
  Samples.push_back("VBFHToBB_M_125");
  //  Samples.push_back("TTToHadronic_TuneCP");
  //  Samples.push_back("TTToSemiLeptonic_TuneCP5");
  //  Samples.push_back("ZJetsToQQ_HT400to600_qc19_4j_TuneCP5");
  //  Samples.push_back("ZJetsToQQ_HT600to800_qc19_4j_TuneCP5");
  //  Samples.push_back("ZJetsToQQ_HT800toInf_qc19_4j_TuneCP5");
  //  Samples.push_back("WJetsToQQ_HT400to600_qc19_4j_TuneCP5");
  //  Samples.push_back("WJetsToQQ_HT600to800_qc19_4j_TuneCP5");
  //  Samples.push_back("WJetsToQQ_HT800toInf_qc19_4j_TuneCP5");

  int NP = Samples.size();

  for (int Nproc=0;Nproc<NP;Nproc++) {
    TString sample = Samples[Nproc];
    TString nameTuple = "Bkg";
    TString dir = "../Making_Ntuples_For_SigBkgDiscrimination/MC_Derived/";
    if (sample=="Full_Data"||)
      dir = "../Making_Ntuples_For_SigBkgDiscrimination/Data_Derived/";
    if (sample=="GluGluHToBB_M125"||sample=="VBFHToBB_M_125")
      nameTuple = "Sig";
      CreateMbbBdtNtuple(dir,sample,nameTuple);
  }


}
