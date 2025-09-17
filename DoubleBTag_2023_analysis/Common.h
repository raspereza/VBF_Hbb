#include "TH1.h"
#include <string>
double luminosity = 1.0;

std::map<TString,TString> label = {
    {"Nom",    "Nominal  "},
    {"JECUp",  "JECUp    "},
    {"JECDown","JECDown  "},
    {"JECUp",  "PNJECUp  "},
    {"JECDown","PNJECDown"},
    {"JERUp",  "JERUp    "},
    {"JERDown","JERDown  "},
};

TString variable = "T_reg_mbb";
TString variableData = "T_reg_mbb";

/*
TString cuts[4] = {
                    "(DNN_BiClass >=0.90 && DNN_BiClass < 0.95) && (T_btgb1 > 0.2605 && T_btgb2 > 0.2605)",
                    "(DNN_BiClass >=0.95 && DNN_BiClass < 0.975) && (T_btgb1 > 0.2605 && T_btgb2 > 0.2605)",
                    "(DNN_BiClass >=0.975) && (T_btgb1 > 0.2605 && T_btgb2 > 0.2605)" };

*/
 TString cuts[7] =
{
    "(DNN_VBF / (DNN_VBF + DNN_QCD + DNN_TT) >= 0.90 && DNN_VBF / (DNN_VBF + DNN_QCD + DNN_TT) < 0.95)  && (T_btgb1 > 0.2605 && T_btgb2 > 0.2605)",
    "(DNN_VBF / (DNN_VBF + DNN_QCD + DNN_TT) >= 0.95 && DNN_VBF / (DNN_VBF + DNN_QCD + DNN_TT) < 0.975) && (T_btgb1 > 0.2605 && T_btgb2 > 0.2605) ",
    "(DNN_VBF / (DNN_VBF + DNN_QCD + DNN_TT) >= 0.9750) && (T_btgb1 > 0.2605 && T_btgb2 > 0.2605) ",
    "(DNN_GGH / (DNN_GGH + DNN_QCD + DNN_TT) >= 0.80  && DNN_GGH / (DNN_GGH + DNN_QCD + DNN_TT) < 0.90)    &&  (DNN_VBF / (DNN_VBF + DNN_QCD + DNN_TT) < 0.90) && (T_btgb1 > 0.2605 && T_btgb2 > 0.2605)",
    "(DNN_GGH / (DNN_GGH + DNN_QCD + DNN_TT) >= 0.90) && (DNN_VBF / (DNN_VBF + DNN_QCD + DNN_TT) < 0.90)   &&  (T_btgb1 > 0.2605 && T_btgb2 > 0.2605)",
    "(DNN_Z2Q / (DNN_Z2Q + DNN_QCD + DNN_TT) >= 0.80  && DNN_Z2Q / (DNN_Z2Q + DNN_QCD + DNN_TT) < 0.90)    &&  (DNN_GGH / (DNN_GGH + DNN_QCD + DNN_TT) < 0.80) && (DNN_VBF / (DNN_VBF + DNN_QCD + DNN_TT) < 0.90) && (T_btgb1 > 0.2605 && T_btgb2 > 0.2605)",
    "(DNN_Z2Q / (DNN_Z2Q + DNN_QCD + DNN_TT) >= 0.90) &&  (DNN_GGH / (DNN_GGH + DNN_QCD + DNN_TT) < 0.80) &&  (DNN_VBF / (DNN_VBF + DNN_QCD + DNN_TT) < 0.90) && (T_btgb1 > 0.2605 && T_btgb2 > 0.2605)",
};
TString dipole[4] = {"0.0", "0.01882187502",      "0.06886822505" ,  "0.1226316401"}; 

TString dirName =  "/eos/home-s/sapradha/PhD_Projects/VBF_analysis/CMSSW_14_0_9/src/VBFHBB/Analysis/test/FINAL_ANALYSIS/DNN_Ntuples_Train/2023_Ntuples_";  // use this dir for 92% and ANN

TString FileNamesVBFDNN[9] = {
  "tree_VBFHto2B_M-125_TuneCP5_13p6TeV_powheg-pythia8_2023_btgsf.root",            // central
  "tree_VBFHto2B_M-125_TuneCP5_13p6TeV_powheg-pythia8_2023_JECUp_btgsf.root",      // JEC Up
  "tree_VBFHto2B_M-125_TuneCP5_13p6TeV_powheg-pythia8_2023_JECDown_btgsf.root",    // JEC Down
  "tree_VBFHto2B_M-125_TuneCP5_13p6TeV_powheg-pythia8_2023_JERUp_btgsf.root",      // JER Up
  "tree_VBFHto2B_M-125_TuneCP5_13p6TeV_powheg-pythia8_2023_JERDown_btgsf.root",    // JER Down
  "tree_VBFHto2B_M-125_TuneCP5_13p6TeV_powheg-pythia8_2023_PNetJECUp_btgsf.root",  // PNet JEC Up
  "tree_VBFHto2B_M-125_TuneCP5_13p6TeV_powheg-pythia8_2023_PNetJECDown_btgsf.root",// PNet JEC Down
  "tree_VBFHto2B_M-125_TuneCP5_13p6TeV_powheg-pythia8_2023_PNetJECUp_btgsf.root",  // duplicate
  "tree_VBFHto2B_M-125_TuneCP5_13p6TeV_powheg-pythia8_2023_PNetJECDown_btgsf.root" // duplicate
};

TString FileNamesGGHDNN[9] = {
  "tree_GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2023_btgsf.root",            // central
  "tree_GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2023_JECUp_btgsf.root",      // JEC Up
  "tree_GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2023_JECDown_btgsf.root",    // JEC Down
  "tree_GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2023_JERUp_btgsf.root",      // JER Up
  "tree_GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2023_JERDown_btgsf.root",    // JER Down
  "tree_GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2023_PNetJECUp_btgsf.root",  // PNet JEC Up
  "tree_GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2023_PNetJECDown_btgsf.root",// PNet JEC Down
  "tree_GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2023_PNetJECUp_btgsf.root",  // duplicate
  "tree_GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2023_PNetJECDown_btgsf.root" // duplicate
};

TString FileNamesZJetDNN[9] = {
  "tree_Zto2Q-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8_2023_btgsf.root",             // central
  "tree_Zto2Q-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8_2023_JECUp_btgsf.root",       // JEC Up
  "tree_Zto2Q-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8_2023_JECDown_btgsf.root",     // JEC Down
  "tree_Zto2Q-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8_2023_JERUp_btgsf.root",       // JER Up
  "tree_Zto2Q-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8_2023_JERDown_btgsf.root",     // JER Down
  "tree_Zto2Q-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8_2023_PNetJECUp_btgsf.root",   // PNet JEC Up
  "tree_Zto2Q-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8_2023_PNetJECDown_btgsf.root", // PNet JEC Down
  "tree_Zto2Q-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8_2023_PNetJECUp_btgsf.root",   // duplicate
  "tree_Zto2Q-4Jets_TuneCP5_13p6TeV_madgraphMLM-pythia8_2023_PNetJECDown_btgsf.root"  // duplicate
};



TString sysNames[9] = {"Nom","JESUp","JESDown","JERUp","JERDown","PNetJECUp","PNetJECDown","SmearUp","SmearDown"};

std::vector<std::string> genericPolynomExp = {
  "TMath::Exp(@1*@0)",
  "TMath::Exp(@2*@0)*(@1*((@0-125.0)/125.0) + 1.0)",
  "TMath::Exp(@3*@0)*(@2*((@0-125.0)/125.0)*((@0-125.0)/125.0) + @1*((@0-125.0)/125.0) + 1.0)",
  "TMath::Exp(@4*@0)*(@3*((@0-125.0)/125.0)*((@0-125.0)/125.0)*((@0-125.0)/125.0)+@2*((@0-125.0)/125.0)*((@0-125.0)/125.0) + @1*((@0-125.0)/125.0) + 1.0)"
};

std::vector<std::string> genericPolynomExp2 = {
  "TMath::Power(@0,@1)*TMath::Exp(@2*@0);",
};


std::vector<std::string> polynomExpNames = {
  "EXP_POL0",
  "EXP_POL1",
  "EXP_POL2",
  "EXP_POL3"
};

std::vector<std::string> genericPolynomsPOW = {
  "TMath::Exp(@1*@0)",
  "TMath::Exp(@1*@0) + @2*TMath::Exp(@3*@0*@0)",
  "TMath::Exp(@1*@0) + @2*TMath::Exp(@3*@0*@0) + @4*TMath::Exp(@5*@0*@0)",
  "TMath::Exp(@1*@0) + @2*TMath::Exp(@3*@0*@0) + @4*TMath::Exp(@5*@0*@0) + @6*TMath::Exp(@7*@0*@0)",
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
  
TString names[9] = {
  "DoubleB0_2023",
  "DoubleB1_2023",
  "DoubleB2_2023",
  "DoubleB3_2023",
  "DoubleB4_2023",
  "DoubleB5_2023",
  "DoubleB6_2023",
  "DoubleB7_2023",
  "DoubleB8_2023",
};

std::vector<std::string> namesCAT = {
  "DoubleB0_2023",
  "DoubleB1_2023",
  "DoubleB2_2023",
  "DoubleB3_2023",
  "DoubleB4_2023",
  "DoubleB5_2023",
  "DoubleB6_2023",
  "DoubleB7_2023",
  "DoubleB8_2023",
}; // needed to define transfer functions

int nCATs = 4;

//double znorm[5] = {1.0,1.0,3.51,1.,1.,1.};
double znorm[9] = {1.,1.,1.,1.,1.,1.,1.,1.,1.};

int NbinsBkg = 60;
int NbinsSig = 24;
int Nbins = 1500;

float xmin = 80;
float xmax = 200;

float xmin_mc = 80;
float xmax_mc = 200;

float xmin_blind = 104;
float xmax_blind = 146;

int Nbins_wj = 14;
float xmin_wj = 60;
float xmax_wj = 200;

TString Xmin_blind("104");
TString Xmax_blind("146");


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
