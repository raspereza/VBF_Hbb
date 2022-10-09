#include "Common.h"
void Numbers(int iCAT=1) {

  std::map<TString, std::vector<TString> > samples = {
    {"ggH",{"Mass_and_BDT_ggF_Hbb_POWHEG"}},
    {"qqH",{"Mass_and_BDT_VBF_Hbb_Dipol"}},
    {"DY",{"Mass_and_BDT_ZJets","Mass_and_BDT_ZJets_EWK"}},
    {"Top",{"Mass_and_BDT_Sandgle_Top","Mass_and_BDT_tt"}},
    {"W",{"Mass_and_BDT_WJets"}},
    {"Data",{"Mass_and_BDT_DATA"}}
  };

  TFile * file = new TFile("ntuples/mbb_and_bdt_all_Nom.root");
  for (auto sample : samples) {
    TString name = sample.first;
    std::vector<TString> sampleNames = sample.second;
    TH1D * hist = new TH1D("hist_"+name,"",60,80,200);
    for (auto sampleName : sampleNames) {
      TNtuple * tuple = (TNtuple*)file->Get(sampleName);
      TString histName = sampleName + "_hist";
      //      std::cout << histName << std::endl;
      TH1D * histSample = new TH1D(histName,"",60,80,200);
      if (name=="Data")
	tuple->Draw(variableData+">>"+histName,cuts[iCAT]);
      else if (name=="W"||name=="Top")
	tuple->Draw(variableData+">>"+histName,"weight*("+cuts[iCAT]+")");
      else
	tuple->Draw(variable+">>"+histName,"weight*("+cuts[iCAT]+")");
      hist->Add(hist,histSample,1.,1.);
    }
    double err2 = 0;
    double yield = 0;
    for (int iB=1; iB<=60; ++iB) {
      double x = hist->GetBinContent(iB);
      double err = hist->GetBinError(iB);
      yield += x;
      err2 += err*err;
    }
    double yieldE = TMath::Sqrt(err2);
    std::cout << name << " : " << yield << " +/- " << yieldE << std::endl;

  }


}
