#include "HttStylesNew.cc"
#include "Common.h"
//**********************************************//
//  Determination of FTest with Chebysev Pol    //
//**********************************************//
void FTestBackgroundQCD_brn(int iCAT=3) {

  using namespace RooFit;
  SetStyle();

  TFile * file = new TFile("mbb_and_bdt_all.root");
  TNtuple * tree = (TNtuple*)file->Get("Mass_and_BDT_DATA");
  TNtuple * wjmc = (TNtuple*)file->Get("Mass_and_BDT_WJets");
  TNtuple * zjmc = (TNtuple*)file->Get("Mass_and_BDT_ZJets");
  TNtuple * ttmc  = (TNtuple*)file->Get("Mass_and_BDT_tt");

  TH1::SetDefaultSumw2(true);
  TH1D * hist[5];
  TH1D * hist_wj[5];
  TH1D * hist_zj[5];
  TH1D * hist_tt[5];
  TH1D * ratioHist[5];
  TString names[5] = {"CAT0","CAT1","CAT2","CAT3"};
  TCanvas * dummy = new TCanvas("dummy","",800,700);
  for (int i=0; i<4; ++i) {
    TString nameHist = "mbb_"+names[i];

    TString nameHist_wj = "mbb_wj"+names[i];
    TString nameHist_zj = "mbb_zj"+names[i];
    TString nameHist_tt = "mbb_tt"+names[i];

     TString nameRatioHist = "mbb_ratio_"+names[i];
    hist[i] = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
    hist_wj[i] = new TH1D(nameHist_wj,"",NbinsBkg,xmin,xmax);
    hist_zj[i] = new TH1D(nameHist_zj,"",NbinsBkg,xmin,xmax);
    hist_tt[i] = new TH1D(nameHist_tt,"",NbinsBkg,xmin,xmax);
  
    ratioHist[i] = new TH1D(nameRatioHist,"",NbinsBkg,xmin,xmax);
    tree->Draw("mbb>>"+nameHist,cuts[i]);
    wjmc->Draw("mbb>>"+nameHist_wj,"weight*("+cuts[i]+")");
    zjmc->Draw("mbb>>"+nameHist_zj,"weight*("+cuts[i]+")");
    ttmc->Draw("mbb>>"+nameHist_tt,"weight*("+cuts[i]+")");
  }

  delete dummy;
  RooRealVar mbb("mbb","mass(bb)",xmin,xmax);
  RooRealVar b0("b0","b0",0,1);
  RooRealVar b1("b1","b1",0,1);
  RooRealVar b2("b2","b2",0,1);
  RooRealVar b3("b3","b3",0,1);
  RooRealVar b4("b4","b4",0,1);
  RooRealVar b5("b5","b5",0,1);

  hist[iCAT]->Add(hist_wj[iCAT],-1);
  hist[iCAT]->Add(hist_zj[iCAT],-1);
  hist[iCAT]->Add(hist_tt[iCAT],-1);



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
    RooBernstein BRN("Bernstein","Bernstein",mbb,argList);
    RooFitResult * fitRes = BRN.fitTo(data);
    RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",BRN,data) ;
    ndof[j] = NbinsBkg - j;
    chi2[j] = chi2Roo.getVal();
    prob[j] = TMath::Prob(chi2[j],ndof[j]);
    double numerator = chi2[j-1]-chi2[j];
    double denominator = chi2[j]/(NbinsBkg-j);
    double h = numerator/denominator;
    ftest[j] = TMath::FDistI(h,1,NbinsBkg-j);
  }
  std::cout << cuts[iCAT] << std::endl;
  for (int j=2; j<7; ++j)
    cout << "BRN order " << j << "  Chi2 = " << chi2[j] << "  Prob = " << prob[j] << endl; 
}
