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

TString VBFdisc = "(DNN_VBF / (DNN_VBF + DNN_QCD + DNN_TT))";
TString GGHdisc = "(DNN_GGH / (DNN_GGH + DNN_QCD + DNN_TT))";
TString ZQQdisc = "(DNN_Z2Q/(DNN_QCD + DNN_Z2Q))";
TString btagCutEra =
"((era == 0 && T_btgb1 > 0.245  && T_btgb2 > 0.245) || "
 "(era == 1 && T_btgb1 > 0.2605 && T_btgb2 > 0.2605))";
TString cuts[7] =
{
    // 1) VBF very tight
    "((" + VBFdisc + " >= 0.95) && " + btagCutEra + ")",

    // 2) VBF medium
    "((" + VBFdisc + " >= 0.90 && " + VBFdisc + " < 0.95) && " + btagCutEra + ")",

    // 3) VBF loose
    "((" + VBFdisc + " >= 0.85 && " + VBFdisc + " < 0.9) && " + btagCutEra + ")",

    // 4) GGH tight (veto VBF)
    "((" + GGHdisc + " >= 0.78)"
    " && (" + VBFdisc + " < 0.85) && " + btagCutEra + ")",

    // 5) GGH loose (veto VBF)
    "((" + GGHdisc + " >= 0.73 && " + GGHdisc + " < 0.780)"
    " && (" + VBFdisc + " < 0.85) && " + btagCutEra + ")",

    // 6) ZQQ tight (veto GGH & VBF)
    "((" + ZQQdisc + " >= 0.820)"
    " && (" + GGHdisc + " < 0.730)"
    " && (" + VBFdisc + " < 0.85) && " + btagCutEra + ")",

    // 7) ZQQ loose (veto GGH & VBF)
    "((" + ZQQdisc + " >= 0.77 && " + ZQQdisc + " < 0.820)"
    " && (" + GGHdisc + " < 0.73)"
    " && (" + VBFdisc + " < 0.85) && " + btagCutEra + ")"
};
/*
TString cuts[7] =
{
    // 1) VBF very tight
    "((" + VBFdisc + " >= 0.95) && " + btagCutEra + ")",

    // 2) VBF medium
    "((" + VBFdisc + " >= 0.90 && " + VBFdisc + " < 0.95) && " + btagCutEra + ")",

    // 3) VBF loose
    "((" + VBFdisc + " >= 0.80 && " + VBFdisc + " < 0.9) && " + btagCutEra + ")",

    // 4) GGH tight (veto VBF)
    "((" + GGHdisc + " >= 0.82)"
    " && (" + VBFdisc + " < 0.8) && " + btagCutEra + ")",

    // 5) GGH loose (veto VBF)
    "((" + GGHdisc + " >= 0.75 && " + GGHdisc + " < 0.820)"
    " && (" + VBFdisc + " < 0.80) && " + btagCutEra + ")",

    // 6) ZQQ tight (veto GGH & VBF)
    "((" + ZQQdisc + " >= 0.820)"
    " && (" + GGHdisc + " < 0.75)"
    " && (" + VBFdisc + " < 0.8) && " + btagCutEra + ")",

    // 7) ZQQ loose (veto GGH & VBF)
    "((" + ZQQdisc + " >= 0.75 && " + ZQQdisc + " < 0.820)"
    " && (" + GGHdisc + " < 0.75)"
    " && (" + VBFdisc + " < 0.8) && " + btagCutEra + ")"
};
*/

/*
TString cuts[7] =
{
    // 1) VBF very tight
    "((" + VBFdisc + " >= 0.975) && " + btagCutEra + ")",

    // 2) VBF medium
    "((" + VBFdisc + " >= 0.95 && " + VBFdisc + " < 0.975) && " + btagCutEra + ")",

    // 3) VBF loose
    "((" + VBFdisc + " >= 0.90 && " + VBFdisc + " < 0.95) && " + btagCutEra + ")",

    // 4) GGH tight (veto VBF)
    "((" + GGHdisc + " >= 0.90)"
    " && (" + VBFdisc + " < 0.90) && " + btagCutEra + ")",

    // 5) GGH loose (veto VBF)
    "((" + GGHdisc + " >= 0.80 && " + GGHdisc + " < 0.90)"
    " && (" + VBFdisc + " < 0.90) && " + btagCutEra + ")",

    // 6) ZQQ tight (veto GGH & VBF)
    "((" + ZQQdisc + " >= 0.90)"
    " && (" + GGHdisc + " < 0.875)"
    " && (" + VBFdisc + " < 0.90) && " + btagCutEra + ")",

    // 7) ZQQ loose (veto GGH & VBF)
    "((" + ZQQdisc + " >= 0.80 && " + ZQQdisc + " < 0.90)"
    " && (" + GGHdisc + " < 0.80)"
    " && (" + VBFdisc + " < 0.90) && " + btagCutEra + ")"
};
*/

TString dipole[4] = {"0.0", "0.01882187502",      "0.06886822505" ,  "0.1226316401"};
TString dirName =  "/eos/home-s/sapradha/PhD_Projects/VBF_analysis/CMSSW_14_0_9/src/VBFHBB/Analysis/test/FINAL_ANALYSIS/DNN_Ntuples_Train/Run_3_DNN/Ntuples_with_DNN_score/2022";

TString FileNamesVBFDNN[9] = {
    "VBFHto2B_M-125_dipoleRecoilOn_TuneCP5_13p6TeV_powheg-pythia8_2022preEE_2022EE.root",
    "VBFHto2B_M-125_dipoleRecoilOn_TuneCP5_13p6TeV_powheg-pythia8_2022preEE_2022EE_JECUp.root",
    "VBFHto2B_M-125_dipoleRecoilOn_TuneCP5_13p6TeV_powheg-pythia8_2022preEE_2022EE_JECDown.root",
    "VBFHto2B_M-125_dipoleRecoilOn_TuneCP5_13p6TeV_powheg-pythia8_2022preEE_2022EE_JERUp.root",
    "VBFHto2B_M-125_dipoleRecoilOn_TuneCP5_13p6TeV_powheg-pythia8_2022preEE_2022EE_JERDown.root",
    "VBFHto2B_M-125_dipoleRecoilOn_TuneCP5_13p6TeV_powheg-pythia8_2022preEE_2022EE_PNetJECUp.root",
    "VBFHto2B_M-125_dipoleRecoilOn_TuneCP5_13p6TeV_powheg-pythia8_2022preEE_2022EE_PNetJECDown.root",
    "VBFHto2B_M-125_dipoleRecoilOn_TuneCP5_13p6TeV_powheg-pythia8_2022preEE_2022EE_PNetJECUp.root",
    "VBFHto2B_M-125_dipoleRecoilOn_TuneCP5_13p6TeV_powheg-pythia8_2022preEE_2022EE_PNetJECDown.root"
};

TString FileNamesGGHDNN[9] = {
    "GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2022preEE_2022EE.root",
    "GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2022preEE_2022EE_JECUp.root",
    "GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2022preEE_2022EE_JECDown.root",
    "GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2022preEE_2022EE_JERUp.root",
    "GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2022preEE_2022EE_JERDown.root",
    "GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2022preEE_2022EE_PNetJECUp.root",
    "GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2022preEE_2022EE_PNetJECDown.root",
    "GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2022preEE_2022EE_PNetJECUp.root",
    "GluGluHto2B_M-125_TuneCP5_13p6TeV_powheg-minlo-pythia8_2022preEE_2022EE_PNetJECDown.root"
};

TString FileNamesZJetDNN[9] = {
    "Zto2Q-4Jets_HT-200toinf_TuneCP5_13p6TeV_madgraphMLM-pythia8_2022preEE_2022EE.root",
    "Zto2Q-4Jets_HT-200toinf_TuneCP5_13p6TeV_madgraphMLM-pythia8_2022preEE_2022EE_JECUp.root",
    "Zto2Q-4Jets_HT-200toinf_TuneCP5_13p6TeV_madgraphMLM-pythia8_2022preEE_2022EE_JECDown.root",
    "Zto2Q-4Jets_HT-200toinf_TuneCP5_13p6TeV_madgraphMLM-pythia8_2022preEE_2022EE_JERUp.root",
    "Zto2Q-4Jets_HT-200toinf_TuneCP5_13p6TeV_madgraphMLM-pythia8_2022preEE_2022EE_JERDown.root",
    "Zto2Q-4Jets_HT-200toinf_TuneCP5_13p6TeV_madgraphMLM-pythia8_2022preEE_2022EE_PNetJECUp.root",
    "Zto2Q-4Jets_HT-200toinf_TuneCP5_13p6TeV_madgraphMLM-pythia8_2022preEE_2022EE_PNetJECDown.root",
    "Zto2Q-4Jets_HT-200toinf_TuneCP5_13p6TeV_madgraphMLM-pythia8_2022preEE_2022EE_PNetJECUp.root",
    "Zto2Q-4Jets_HT-200toinf_TuneCP5_13p6TeV_madgraphMLM-pythia8_2022preEE_2022EE_PNetJECDown.root"
};

TString data_file_name = "JetMET_2022preEE_2022EE.root";


TString sysNames[9] = {"Nom","JESUp","JESDown","JERUp","JERDown","PNetJECUp","PNetJECDown","SmearUp","SmearDown"};

// --------------------------- systematic calculation --------------------------

TString Cat_DNN_var_map[7] = {
       VBFdisc , VBFdisc , VBFdisc , GGHdisc , GGHdisc , ZQQdisc ,ZQQdisc
};
TString vbf_weight = "T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_multiWP*T_online_btag_weight*";
TString ggh_weight = "T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_multiWP*T_online_btag_weight*";
TString zjet_weight = "T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_multiWP*T_online_btag_weight*T_V_pTweight_QCD*T_V_pTweight_EWK*";



TString syst[] = {"CMS_pileup_2022",
                          "CMS_btag_lf_2022","CMS_btag_lfstats1_2022","CMS_btag_lfstats2_2022",
                          "CMS_btag_hf_2022","CMS_btag_hfstats1_2022","CMS_btag_hfstats2_2022",
                          "CMS_btag_cferr1_2022","CMS_btag_cferr2_2022"};  // systematic_calculator , index of each systematic is being mapped to the main script
TString up_weights[] = {
        "(T_PUweight_up/T_PUweight)",
        "(T_btag_weight_up_lf/T_btag_weight_multiWP)","(T_btag_weight_up_lfstats1/T_btag_weight_multiWP)","(T_btag_weight_up_lfstats2/T_btag_weight_multiWP)",
        "(T_btag_weight_up_hf/T_btag_weight_multiWP)","(T_btag_weight_up_hfstats1/T_btag_weight_multiWP)","(T_btag_weight_up_hfstats2/T_btag_weight_multiWP)" ,
        "(T_btag_weight_up_cferr1/T_btag_weight_multiWP)" , "(T_btag_weight_up_cferr2/T_btag_weight_multiWP)"

};
TString down_weights[] = {
    "(T_PUweight_down/T_PUweight)",
    "(T_btag_weight_down_lf/T_btag_weight_multiWP)",
    "(T_btag_weight_down_lfstats1/T_btag_weight_multiWP)",
    "(T_btag_weight_down_lfstats2/T_btag_weight_multiWP)",
    "(T_btag_weight_down_hf/T_btag_weight_multiWP)",
    "(T_btag_weight_down_hfstats1/T_btag_weight_multiWP)",
    "(T_btag_weight_down_hfstats2/T_btag_weight_multiWP)",
    "(T_btag_weight_down_cferr1/T_btag_weight_multiWP)",
    "(T_btag_weight_down_cferr2/T_btag_weight_multiWP)"
};


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
  "TF_POL5",
};
  
TString names[7] = {
  "qqh_0_2022",
  "qqh_1_2022",
  "qqh_2_2022",
  "ggh_0_2022",
  "ggh_1_2022",
  "zjet_0_2022",
  "zjet_1_2022",
};

std::vector<std::string> namesCAT = {
  "qqh_0_2022",
  "qqh_1_2022",
  "qqh_2_2022",
  "ggh_0_2022",
  "ggh_1_2022",
  "zjet_0_2022",
  "zjet_1_2022",
}; // needed to define transfer functions
int nCATs = 4;

//double znorm[5] = {1.0,1.0,3.51,1.,1.,1.};
double znorm[9] = {1.,1.,1.,1.,1.,1.,1.,1.,1.};

int NbinsBkg = 60;
int NbinsSig = 24;
int Nbins = 2000;

float xmin = 80;
float xmax = 240;

float xmin_mc = 80;
float xmax_mc = 240;

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
