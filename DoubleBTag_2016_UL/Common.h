#include "TH1.h"
#include <string>

std::map<TString,TString> label = {
    {"Nom",    "Nominal"},
    {"JESUp",  "JESUp  "},
    {"JESDown","JESDown"},
    {"JERUp",  "JERUp  "},
    {"JERDown","JERDown"},
};

TString variable = "mbb_reg";

TString cuts[6] = {
  "bdtout_2>=0.10 && bdtout_2<0.11",
  "bdtout_2>=0.11",
  "bdtout_2<0.1 && bdtout_0>=0.6 && bdtout_0<0.75",
  "bdtout_2<0.1 && bdtout_0>=0.75",
  "bdtout_2<0.1 && bdtout_0<0.6 && bdtout_1>=0.5 && bdtout_1<0.75",
  "bdtout_2<0.1 && bdtout_0<0.6 && bdtout_1>=0.75",
};


TString dirName =  "ntuples";
TString FileNamesBDT[5] = {
  "mbb_and_bdt_all_Nom.root",
  "mbb_and_bdt_all_JEC_up.root",
  "mbb_and_bdt_all_JEC_dn.root",
  "mbb_and_bdt_all_JER_up.root",
  "mbb_and_bdt_all_JER_dn.root"
};

TString sysNames[5] = {"Nom","JESUp","JESDown","JERUp","JERDown"};

std::vector<std::string> genericPolynomExp = {
  "TMath::Exp(@1*@0)",
  "TMath::Exp(@2*@0)*(@1*((@0-125.0)/125.0) + 1.0)",
  "TMath::Exp(@3*@0)*(@2*((@0-125.0)/125.0)*((@0-125.0)/125.0) + @1*((@0-125.0)/125.0) + 1.0)",
  "TMath::Exp(@4*@0)*(@3*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)+@2*((@0-125.0)/125.0)*((@0-125.0)/125.0) + @1*((@0-125.0)/125.0) + 1.0)"
};

std::vector<std::string> polynomExpNames = {
  "EXP_POL0",
  "EXP_POL1",
  "EXP_POL2",
  "EXP_POL3"
};


std::vector<std::string> genericPolynoms = {
  "1",
  "@1*((@0-125.0)/125.0) + 1",
  "@2*((@0-125.0)/125.0)*((@0-125.0)/125.0) + @1*((@0-125.0)/125.0) + 1",
  "@3*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)+@2*((@0-125.0)/125.0)*((@0-125.0)/125.0) + @1*((@0-125.0)/125.0) + 1",
  "@4*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)+@3*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)+@2*((@0-125.0)/125.0)*((@0-125.0)/125.0) + @1*((@0-125.0)/125.0) + 1",
  "@5*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)+@4*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)+@3*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)+@2*((@0-125.0)/125.0)*((@0-125.0)/125.0) + @1*((@0-125.0)/125.0) + 1"
};

std::vector<std::string> polynomNames = {
  "TF_POL0",
  "TF_POL1",
  "TF_POL2",
  "TF_POL3",
  "TF_POL4",
};
  
TString names[6] = {
  "DoubleB1_2016",
  "DoubleB2_2016",
  "DoubleB3_2016",
  "DoubleB4_2016",
  "DoubleB5_2016",
  "DoubleB6_2016"
};

std::vector<std::string> namesCAT = {
  "DoubleB1_2016",
  "DoubleB2_2016",
  "DoubleB3_2016",
  "DoubleB4_2016",
  "DoubleB5_2016",
  "DoubleB6_2016"
}; // needed to define transfer functions

int nCATs = 6;

//double znorm[5] = {1.0,1.0,3.51,1.,1.,1.};
double znorm[6] = {1.,1.,1.,1.,1.,1.};

int NbinsBkg = 60;
int NbinsSig = 30;
int Nbins = 1200;

float xmin = 80;
float xmax = 200;

float xmin_mc = 80;
float xmax_mc = 200;

float xmin_blind = 104;
float xmax_blind = 146;

TString Xmin_blind("104");
TString Xmax_blind("146");

int Nbins_wj = 16;

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
      //      double xratioE = xratio*xratioR;
      double xratioE = xNumE/xDen;
      ratioH->SetBinContent(iB,xratio);
      ratioH->SetBinError(iB,xratioE);
    }
  }

  return ratioH;
}
