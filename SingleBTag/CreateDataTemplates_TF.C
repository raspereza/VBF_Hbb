#include "Common.h"

using namespace RooFit;

void CreateDataTemplates_TF() {

  int iORDER = 5; // Order of BRN polynome for baseline function 
  int iorder[5] = {3,3,3,2,2}; // order of BRN polynomes in transfer functions

  TFile * file = new TFile("Full_Data_stat.root");
  TTree * tree = (TTree*)file->Get("Stat");

  TFile * fileOutput = new TFile("data_singleb_shapes_tf.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","data");
  
  TH1::SetDefaultSumw2(true);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_"+names[0];
  TH1D * hist = new TH1D(nameHist,"",Nbins,xmin,xmax);
  tree->Draw("mbb>>"+nameHist,cuts[0]);
  delete dummy;

  RooRealVar mbb0("mbb_"+names[0],"mass(bb)",80,200);
  RooRealVar b0("b0_"+names[0],"b0",1.);
  RooRealVar b1("b1_"+names[0],"b1",1.);
  RooRealVar b2("b2_"+names[0],"b2",1.);
  RooRealVar b3("b3_"+names[0],"b3",1.);
  RooRealVar b4("b4_"+names[0],"b4",1.);
  RooRealVar b5("b5_"+names[0],"b5",1.);

  RooArgList argList(b0,b1,b2);
  if (iORDER>=4)
    argList.add(b3);
  if (iORDER>=5)
    argList.add(b4);
  if (iORDER>=6)
    argList.add(b5);
  
  RooBernstein BRN("qcd_"+names[0],"qcd_"+names[0],mbb0,argList);
  RooDataHist data("data_"+names[0],"data",mbb0,hist);

  Float_t yield = hist->GetSumOfWeights();
  RooRealVar qcd_yield("qcd_yield_"+names[0],"Yield",yield,0.5*yield,2*yield);
  
  RooFitResult * fitRes = BRN.fitTo(data);  
  w->import(BRN);
  w->import(data);
  w->import(qcd_yield);

  for (int iCAT=0; iCAT<5; ++iCAT) {

    TCanvas * dummy = new TCanvas("dummy","",800,700);
    TString nameHist = "mbb_"+names[iCAT];
    TString nameRatHist = "mbb_ratio_"+names[iCAT];

    TH1D * hist = new TH1D(nameHist,"",Nbins,80,200);
    TH1D * ratio = new TH1D(nameRatHist,"",Nbins,80,200);
    tree->Draw("mbb>>"+nameHist,cuts[iCAT]);
    
    TString nameHist0 = "mbb_base_"+names[0];
    TH1D * hist0 = new TH1D(nameHist0,"",Nbins,80,200);
    tree->Draw("mbb>>"+nameHist0,cuts[0]);
    delete dummy;

    double norm = hist->GetSumOfWeights();
    double norm0 = hist0->GetSumOfWeights();
    
    
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
	ratio->SetBinContent(iB,xratio);
	ratio->SetBinError(iB,xratioE);
      }
    }

    RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",80,200);
    RooRealVar a0("a0_"+names[iCAT],"a0",1.);
    RooRealVar a1("a1_"+names[iCAT],"a1",1.);
    RooRealVar a2("a2_"+names[iCAT],"a2",1.);
    RooRealVar a3("a3_"+names[iCAT],"a3",1.);
    RooRealVar a4("a4_"+names[iCAT],"a4",1.);
    RooArgList argListTF(a0,a1);
    if (iorder[iCAT]>=2) {
      argListTF.add(a2);
    }
    if (iorder[iCAT]>=3) {
      argListTF.add(a3);
    }
    if (iorder[iCAT]>=4) {
      argListTF.add(a4);
    }

    RooBernstein tf("tf_"+names[iCAT],"Bernstein",mbb,argListTF);
    RooDataHist dataRatio("data_ratio_"+names[iCAT],"data",mbb,ratio);
    RooDataHist data("data_"+names[iCAT],"data",mbb,hist);
    RooFitResult * fitRes = tf.fitTo(dataRatio);  

    RooBernstein baseFunc("qcd_aux_"+names[iCAT],"aux",mbb,argList);
    RooProdPdf qcd("qcd_"+names[iCAT],"qcd"+names[iCAT],RooArgSet(baseFunc,tf));
    Float_t yield = hist->GetSumOfWeights();
    RooRealVar qcd_yield("qcd_yield_"+names[iCAT],"Yield",yield,0.5*yield,2*yield);
  
    w->import(qcd);
    w->import(data);
    w->import(qcd_yield);
  }

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
