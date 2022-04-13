#include "HttStylesNew.cc"
#include "Common_sys.h"
#include "TMath.h"

void FTestTransferFunctions(int iCAT=2) {

  SetStyle();

  if (iCAT<1&&iCAT>3) {
    std::cout << "Invalid category : " << iCAT << std::endl;
  }

  using namespace RooFit;
  SetStyle();

  TFile * file = new TFile("mbb_and_bdt_all_Nom.root");
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
    tree->Draw("mbb_reg>>"+nameHist,cuts[i]);
    wjmc->Draw("mbb_reg>>"+nameHist_wj,"weight*("+cuts[i]+")");
    zjmc->Draw("mbb_reg>>"+nameHist_zj,"weight*("+cuts[i]+")");
    ttmc->Draw("mbb_reg>>"+nameHist_tt,"weight*("+cuts[i]+")");
  }

  delete dummy;
  hist[0]->Add(hist_wj[0],-1);
  hist[0]->Add(hist_zj[0],-1);
  hist[0]->Add(hist_tt[0],-1);

  hist[iCAT]->Add(hist_wj[iCAT],-1);
  hist[iCAT]->Add(hist_zj[iCAT],-1);
  hist[iCAT]->Add(hist_tt[iCAT],-1);

  TH1D * ratio = DivideHist(hist[iCAT],hist[0],"ratioH");  
  
  InitData(ratio);
  TCanvas * canv = new TCanvas("canv","",600,600);
  ratio->Draw("e1");
  canv->Update();

  RooRealVar mbb("mbb","mass(bb)",xmin,xmax);
  RooRealVar a1("a1","a1",-4,4);
  RooRealVar a2("a2","a2",-4,4);
  RooRealVar a3("a3","a3",-4,4);
  RooRealVar a4("a4","a4",-4,4);

  RooDataHist data("data","data_",mbb,ratio);
  double prob[5]   = {  0,  0,  0,  0,  0};
  double chi2[5]   = {  0,  0,  0,  0,  0};
  double ndof[5]   = {  1,  1,  1,  1,  1};
  double ftest[5]  = {  1,  1,  1,  1,  1};
  for (int j=1; j<4; ++j) {
    RooArgList argList = RooArgList(a1);
    if (j>=2)
      argList.add(a2);
    if (j>=3)
      argList.add(a3);
    if (j>=4)
      argList.add(a4);
    RooGenericPdf Polynom(polynomNames.at(j).c_str(),genericPolynoms.at(j).c_str(),RooArgSet(mbb,argList));    
    RooFitResult * fitRes = fitRes = Polynom.fitTo(data,Save(),SumW2Error(kTRUE));
    //    RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",Polynom,data) ;
    RooAbsReal* intOfFunc = Polynom.createIntegral(mbb);
    double integral = 0;
    double norm = 0;
    for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
      double xCenter = ratio->GetBinCenter(iBin);
      mbb.setVal(xCenter);      
      integral += Polynom.getVal();
      norm += ratio->GetBinContent(iBin);
    }
    std::cout << "integral (model) = " << integral << std::endl;
    std::cout << "integral (th1)   = " << norm << std::endl;
    double scale = norm/integral;
    for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
      double xCenter = ratio->GetBinCenter(iBin);
      double xRatio  = ratio->GetBinContent(iBin);
      double xError  = ratio->GetBinError(iBin);
      mbb.setVal(xCenter);
      double xModel  = scale*Polynom.getVal();
      double term = (xRatio-xModel)/xError;
      std::cout << iBin << "  th1 = " << xRatio << "   model = " << xModel << "  error = " << xError << std::endl;
      chi2[j] += term*term;
    }
    ndof[j] = NbinsBkg - j;
    //    chi2[j] = chi2Roo.getVal();
    prob[j] = TMath::Prob(chi2[j],ndof[j]);
    double numerator = chi2[j-1]-chi2[j];
    double denominator = chi2[j]/(NbinsBkg-j);
    double h = numerator/denominator;
    ftest[j] = TMath::FDistI(h,1,NbinsBkg-j);
  }
  for (int j=1; j<4; ++j)
    cout << "polynom order " << j << "  Chi2 = " << chi2[j] << "   ndof = " << ndof[j] << "  Prob = " << prob[j] << endl; 
}
