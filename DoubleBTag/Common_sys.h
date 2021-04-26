#include "TH1.h"
#include <string>

std::map<TString,TString> label = {
    {"Nom",    "Nominal"},
    {"JESUp",  "JESUp  "},
    {"JESDown","JESDown"},
    {"JERUp",  "JERUp  "},
    {"JERDown","JERDown"},
};

TString cuts[5] = {"bdtout<0.3",
		   "bdtout>=0.3&&bdtout<0.6",
		   "bdtout>=0.6&&bdtout<0.8",
		   "bdtout>=0.8"};

TString dirName = "/afs/cern.ch/user/t/tumasyan/public/For_Soumya/Overlap_Removed_DoubleBTag/Systematics_ROOTS";
TString FileNamesBDT[5] = {
  "mbb_and_bdt_all_Central_JER.root",
  "mbb_and_bdt_all_Up_JEC.root",
  "mbb_and_bdt_all_Down_JEC.root",
  "mbb_and_bdt_all_Up_JER.root",
  "mbb_and_bdt_all_Down_JER.root"
};

TString sysNames[5] = {"Nom","JESUp","JESDown","JERUp","JERDown"};

std::vector<std::string> genericPolynoms = {
  "1",
  "@1*((@0-125.0)/50.0) + 1",
  "@2*((@0-125.0)/50.0)*((@0-125.0)/50.0) + @1*((@0-125.0)/50.0) + 1",
  "@3*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)+@2*((@0-125.0)/50.0)*((@0-125.0)/50.0) + @1*((@0-125.0)/50.0) + 1",
  "@4*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)+@3*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)+@2*((@0-125.0)/50.0)*((@0-125.0)/50.0) + @1*((@0-125.0)/50.0) + 1",
  "@5*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)+@4*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)+@3*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)+@2*((@0-125.0)/50.0)*((@0-125.0)/50.0) + @1*((@0-125.0)/50.0) + 1"
};

std::vector<std::string> polynomNames = {
  "TF_POL0",
  "TF_POL1",
  "TF_POL2",
  "TF_POL3",
  "TF_POL4",
};
  
TString names[5] = {"CAT0",
		    "CAT1",
		    "CAT2",
		    "CAT3"};

std::vector<std::string> namesCAT = {
  "CAT0",
  "CAT1",
  "CAT2",
  "CAT3"
}; // needed to define transfer functions

int NbinsBkg = 150;
int NbinsSig = 30;
int Nbins = 1500;
float xmin = 80;
float xmax = 230;

TH1D * DivideHist(TH1D* hist, TH1D* hist0, TString Name) {

  double norm = hist->GetSumOfWeights();
  double norm0 = hist0->GetSumOfWeights();
    
  TH1D * ratioH = (TH1D*)hist->Clone(Name);
    
  int nBinsX = hist0->GetNbinsX();
  for (int iB=1; iB<=nBinsX; ++iB) {
    double xNum  = hist->GetBinContent(iB)/norm;
    double xNumE = hist->GetBinError(iB)/norm;
    double xDen  = hist0->GetBinContent(iB)/norm0;
    double xDenE = hist0->GetBinError(iB)/norm0;
    double NumR = xNumE/xNum;
    double DenR = xDenE/xDen;
    if (xNum>0&&xDen>0) {
      double xratio  = xNum/xDen;
      double xratioR = TMath::Sqrt(NumR*NumR+DenR*DenR);
      double xratioE = xratio*xratioR;
      ratioH->SetBinContent(iB,xratio);
      ratioH->SetBinError(iB,xratioE);
    }
  }

  return ratioH;
}
