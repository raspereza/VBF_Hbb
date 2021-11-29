#include "TH1.h"
#include <string>

std::map<TString,TString> label = {
    {"Nom",    "Nominal"},
    {"JESUp",  "JESUp  "},
    {"JESDown","JESDown"},
    {"JERUp",  "JERUp  "},
    {"JERDown","JERDown"},
};

TString cuts[5] = {"bdtout<0.0",
		   "bdtout>=0.0 && bdtout<0.3",
		   "bdtout>=0.3 && bdtout<0.5",
		   "bdtout>=0.5"};


TString dirName = "ntuple/"; 
TString FileNamesBDT[5] = {
  "mbb_and_bdt_all_Nom.root",
  "mbb_and_bdt_all_JEC_up.root",
  "mbb_and_bdt_all_JEC_dn.root",
  "mbb_and_bdt_all_JER_up.root",
  "mbb_and_bdt_all_JER_dn.root"
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

TString names[5] = {"CAT4",
                    "CAT5",
                    "CAT6",
                    "CAT7"};

std::vector<std::string> namesCAT = {
  "CAT4",
  "CAT5",
  "CAT6",
  "CAT7"
}; // needed to define transfer functions

int NbinsBkg = 45;
int NbinsSig = 18;

float xmin = 60;
float xmax = 150;

int NbinsSig_wj = 15;
float xmin_wj = 50;
float xmax_wj = 200;

int NbinsSig_tt = 30;
float xmin_tt = 50;
float xmax_tt = 200;

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
