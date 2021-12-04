#include "TH1.h"
#include <string>

TString cuts[5] = {"bdtout<1.0",
                   "bdtout>=0.0 && bdtout<0.3",
                   "bdtout>=0.3 && bdtout<0.5",
                   "bdtout>=0.5 && bdtout<0.7",
                   "bdtout>=0.7"};

std::vector<std::string> genericPolynoms = {
  "1",
  "@1*((@0-125.0)/50.0) + 1",
  "@2*((@0-125.0)/50.0)*((@0-125.0)/50.0) + @1*((@0-125.0)/50.0) + 1",
  "@3*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)+@2*((@0-125.0)/50.0)*((@0-125.0)/50.0) + @1*((@0-125.0)/50.0) + 1",
  "@4*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)+@3*((@0-125.0)/50.0)*((@0-125.0)/50.0)*((@0-125.0)/50.0)+@2*((@0-125.0)/50.0)*((@0-125.0)/50.0) + @1*((@0-125.0)/50.0) + 1"
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
		    "CAT7",
                    "CAT8"     };

std::vector<std::string> namesCAT = {
  "CAT4",
  "CAT5",
  "CAT6",
  "CAT7",
  "CAT8"
}; // needed to define transfer functions

int NbinsBkg = 150;
int NbinsSig =  30;
int Nbins = 1500;

float xmin = 80;
float xmax = 230;

float xmin_wj = 50;
float xmax_wj = 200;
int NbinsSig_wj =  15;

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
