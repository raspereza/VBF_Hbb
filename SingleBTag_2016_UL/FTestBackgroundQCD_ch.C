#include "HttStylesNew.cc"
#include "Common.h"
//**********************************************//
//  Determination of FTest with Chebysev Pol    //
//**********************************************//
void FTestBackgroundQCD_ch(int iCAT=4) {

  using namespace RooFit;
  SetStyle();

  TFile * file = new TFile(dirName+"/mbb_and_bdt_all_Nom.root");
  TNtuple * tree = (TNtuple*)file->Get("Mass_and_BDT_DATA");
  
  vector<TString> sampleNames = {
    "ZJets",
    "WJets",
    "ZJets",
    "tt",
    "Sandgle_Top",
    "VBF_Hbb_Dipol",
    "ggF_Hbb"
  };
  
  TH1::SetDefaultSumw2(true);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb";

  TH1D * hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  tree->Draw("mbb_reg>>"+nameHist,cuts[iCAT]);

  for (auto sample_name : sample_names) {
    TString nameHist_sample = nameHist + "_" + sample_name;
    TNtuple * tuple = (TNtuple*)file->Get();
    tree->Draw("mbb_reg>>"+nameHist,cuts[iCAT]);
  wjmc->Draw("mbb_reg>>"+nameHist_wj,"weight*("+cuts[iCAT]+")");
  zjmc->Draw("mbb_reg>>"+nameHist_zj,"weight*("+cuts[iCAT]+")");
  ttmc->Draw("mbb_reg>>"+nameHist_tt,"weight*("+cuts[iCAT]+")");
  stmc->Draw("mbb_reg>>"+nameHist_st,"weight*("+cuts[iCAT]+")");

  delete dummy;
  RooRealVar mbb("mbb","mass(bb)",xmin,xmax);
  RooRealVar b0("b0","b0",-1,1);
  RooRealVar b1("b1","b1",-1,1);
  RooRealVar b2("b2","b2",-1,1);
  RooRealVar b3("b3","b3",-1,1);
  RooRealVar b4("b4","b4",-1,1);
  RooRealVar b5("b5","b5",-1,1);

  hist[iCAT]->Add(hist_wj[iCAT],-1);
  hist[iCAT]->Add(hist_zj[iCAT],-1);
  hist[iCAT]->Add(hist_tt[iCAT],-1);
  hist[iCAT]->Add(hist_st[iCAT],-1);

  RooDataHist data("data","data_",mbb,hist[iCAT]);
  double prob[10];
  double chi2[10];
  double ndof[10];
  double ftest[10] = {1,1,1,1,1,1,1,1,1,1};
  for (int j=2; j<7; ++j) {
    RooArgList argList = RooArgList(b0,b1);
    if (j>=3)
      argList.add(b2);
    if (j>=4)
      argList.add(b3);
    if (j>=5) 
      argList.add(b4);
    if (j>=6) 
      argList.add(b5);
    RooChebychev CHV("Chebyshev","Chebyshev",mbb,argList);    
    RooFitResult * fitRes = CHV.fitTo(data);  
    RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",CHV,data) ;
    ndof[j] = NbinsBkg - j;
    chi2[j] = chi2Roo.getVal();
    prob[j] = TMath::Prob(chi2[j],ndof[j]);
    double numerator = chi2[j-1]-chi2[j];
    double denominator = chi2[j]/(NbinsBkg-j);
    double h = numerator/denominator;
    ftest[j] = TMath::FDistI(h,1,NbinsBkg-j);
  }
  cout << endl;
  cout << "+++++++++++++++++ " << names[iCAT] << " +++++++++++++++++++++++" << endl;
  cout << endl;
  for (int j=2; j<7; ++j)
    cout << "CHV order " << j << "  Chi2/ndof = " << chi2[j] << "/" << ndof[j] << " = " << chi2[j]/ndof[j] << "  prob = " << prob[j] << endl; 
  cout << endl;
}
