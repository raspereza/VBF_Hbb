#include "HttStylesNew.cc"
#include "Common.h"
#include "TMath.h"

using namespace RooFit;

void FTestTransferFunctions(int iCAT=4) {

  SetStyle();

  if (iCAT<1||iCAT>5) {
    std::cout << "Invalid category : " << iCAT << std::endl;
    return;
  }

  TH1::SetDefaultSumw2(true);

  TFile * file = new TFile(dirName+"/mbb_and_bdt_all_Nom.root");
  vector<TString> sample_names = {
    "ZJets",
    "WJets",
    "tt",    
    "Sandgle_Top",
    "VBF_Hbb_Dipol",
    "ggF_Hbb"
  };
  
  TNtuple * tree = (TNtuple*)file->Get("Mass_and_BDT_DATA");

  TString nameHist0 = "mbb_"+names[0];
  TH1D * hist0 = new TH1D(nameHist0,"",NbinsBkg,xmin,xmax);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  tree->Draw("mbb_reg>>"+nameHist0,cuts[0]);
    
  for (auto sample_name : sample_names) {
    TNtuple * tuple = (TNtuple*)file->Get("Mass_and_BDT_"+sample_name);
    TString nameHist_sample = nameHist0 + sample_name;
    TH1D * hist0_ = new TH1D(nameHist_sample,"",NbinsBkg,xmin,xmax);
    tuple->Draw("mbb_reg>>"+nameHist_sample,"weight*("+cuts[0]+")");
    hist0->Add(hist0_,-1.);
  }
  
  TString nameHist = "mbb_"+names[iCAT];
  TString nameRatioHist = "mbb_ratio_"+names[iCAT];
  TH1D * hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  tree->Draw("mbb_reg>>"+nameHist,cuts[iCAT]);
    
  for (auto sample_name : sample_names) {
    TNtuple * tuple = (TNtuple*)file->Get("Mass_and_BDT_"+sample_name);
    TString nameHist_sample = nameHist + sample_name;
    TH1D * hist_ = new TH1D(nameHist_sample,"",NbinsBkg,xmin,xmax);
    tuple->Draw("mbb_reg>>"+nameHist_sample,"weight*("+cuts[iCAT]+")");
    hist->Add(hist_,-1.);
  }

  delete dummy;

  TH1D * ratio = DivideHist(hist,hist0,nameRatioHist);  

  double chi2[5];
  double chi2_Roo[5];
  double prob[5];
  double ndof[5];

  for (int j=1; j<5; ++j) {

    RooRealVar mbb("mbb","mass(bb)",xmin,xmax);

    RooRealVar a1("a1","a1",-4,4);
    RooRealVar a2("a2","a2",-4,4);
    RooRealVar a3("a3","a3",-4,4);
    RooRealVar a4("a4","a4",-4,4);
    
    RooDataHist data("data","data_",mbb,ratio);

    RooArgList argList = RooArgList(a1);
    if (j>=2)
      argList.add(a2);
    if (j>=3)
      argList.add(a3);
    if (j>=4)
      argList.add(a4);

    RooGenericPdf Polynom(polynomNames.at(j).c_str(),genericPolynoms.at(j).c_str(),RooArgSet(mbb,argList));    
    RooFitResult * fitRes = fitRes = Polynom.fitTo(data,Save(),SumW2Error(kTRUE));
    RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",Polynom,data) ;
    chi2_Roo[j] = chi2Roo.getVal();
    RooAbsReal* intOfFunc = Polynom.createIntegral(mbb);
    double integral = 0;
    double norm = 0;
    for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
      double xCenter = ratio->GetBinCenter(iBin);
      mbb.setVal(xCenter);      
      integral += Polynom.getVal();
      norm += ratio->GetBinContent(iBin);
    }
    std::cout << "polynom order = " << j << std::endl;
    std::cout << "integral (model) = " << integral << std::endl;
    std::cout << "integral (pol)   = " << norm << std::endl;
    double scale = norm/integral;
    chi2[j] = 0.0;
    for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
      double xCenter = ratio->GetBinCenter(iBin);
      double xRatio  = ratio->GetBinContent(iBin);
      double xError  = ratio->GetBinError(iBin);
      mbb.setVal(xCenter);
      double xModel  = scale*Polynom.getVal();
      double term = (xRatio-xModel)/xError;
      chi2[j] += term*term;
    }
    ndof[j] = NbinsBkg - j;
    prob[j] = TMath::Prob(chi2[j],ndof[j]);
  }
  cout << endl;
  cout << "+++++++++++++++++ " << names[iCAT] << " +++++++++++++++++++++++" << endl;
  cout << endl;
  for (int j=1; j<5; ++j)
    cout << "Polynom order " << j << "  Chi2/ndof = " << chi2[j] << "/" <<  ndof[j] << " = " << chi2[j]/ndof[j] << "  prob = " << prob[j] << endl; 
  cout << endl;

}
