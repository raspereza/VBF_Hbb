#include "HttStylesNew.cc"
#include "Common.h"
void PlotTF(int iCAT=2,
	    int iORDER=2,
	    int nB=60,
	    bool blind = true) {

  using namespace RooFit;

  SetStyle();

  TFile * file = new TFile("Full_Data_stat.root");
  TTree * tree = (TTree*)file->Get("Stat");

  TString cutsBlind;
    cutsBlind = cuts[iCAT] + "&&(mbb<100||mbb>150)";

  TH1::SetDefaultSumw2(true);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_"+names[iCAT];
  TString nameBlindHist = "mbb_blind"+names[iCAT];
  TString nameRatHist = "mbb_ratio_"+names[iCAT];
  TString nameRatBlindHist = "mbb_ratio_blind"+names[iCAT];
  TH1D * hist = new TH1D(nameHist,"",nB,80,200);
  TH1D * histBlind = new TH1D(nameBlindHist,"",nB,80,200);
  TH1D * ratio = new TH1D(nameRatHist,"",nB,80,200);
  TH1D * ratioBlind = new TH1D(nameRatBlindHist,"",nB,80,200);
  tree->Draw("mbb>>"+nameHist,cuts[iCAT]);
  tree->Draw("mbb>>"+nameBlindHist,cutsBlind);

  TString nameHist0 = "mbb_base_"+names[iCAT];
  TH1D * hist0 = new TH1D(nameHist0,"",nB,80,200);
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
    double xNumBlind  = histBlind->GetBinContent(iB)/norm;
    double xNumBlindE = histBlind->GetBinError(iB)/norm;
    double NumR = xNumE/xNum;
    double DenR = xDenE/xDen;
    double NumBlindR = xNumBlindE/xNumBlind;
    if (xNum>0&&xDen>0) {
      double xratio  = xNum/xDen;
      double xratioR = TMath::Sqrt(NumR*NumR+DenR*DenR);
      double xratioE = xratio*xratioR;
      ratio->SetBinContent(iB,xratio);
      ratio->SetBinError(iB,xratioE);
    }
    if (xNumBlind>0&&xDen>0) {
      double xratio  = xNumBlind/xDen;
      double xratioR = TMath::Sqrt(NumBlindR*NumBlindR+DenR*DenR);
      double xratioE = xratio*xratioR;
      ratioBlind->SetBinContent(iB,xratio);
      ratioBlind->SetBinError(iB,xratioE);
    }
    
  }

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",80,200);
  RooRealVar a0("a0_"+names[iCAT],"a0",0,10);
  RooRealVar a1("a1_"+names[iCAT],"a1",0,10);
  RooRealVar a2("a2_"+names[iCAT],"a2",0,10);
  RooRealVar a3("a3_"+names[iCAT],"a3",0,10);
  RooRealVar a4("a4_"+names[iCAT],"a4",0,10);
  RooRealVar a5("a5_"+names[iCAT],"a5",-10,10);
  RooArgList argList(a0,a1);
  if (iORDER>=2) {
    argList.add(a2);
  }
  if (iORDER>=3) {
    argList.add(a3);
  }
  if (iORDER>=4) {
    argList.add(a4);
  }

  RooBernstein tf("tf_"+names[iCAT],"Bernstein",mbb,argList);
  RooDataHist dataRatio("data_ratio_"+names[iCAT],"data",mbb,ratio);
  RooDataHist dataRatioBlind("data_ratio_Blind","data",mbb,ratioBlind);
  RooDataHist data("data_"+names[iCAT],"data",mbb,hist);

  RooFitResult * fitRes = tf.fitTo(dataRatio);  
  RooPlot* xframe = mbb.frame();
  if (blind) 
    dataRatio.plotOn(xframe,LineColor(0),MarkerColor(0));
  else 
    dataRatio.plotOn(xframe);
  tf.plotOn(xframe);
  if (blind) 
    dataRatioBlind.plotOn(xframe);

  RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",tf,dataRatio) ;
  double ndof = nB - iORDER;
  double chi2 = chi2Roo.getVal();
  double prob = TMath::Prob(chi2,ndof);
  cout << "Chi2/ndof = " << chi2 << "/" << ndof << " = " << chi2/ndof << "  prob = " << prob << endl; 
  TCanvas * canv = new TCanvas("canv","",700,700);
  xframe->SetTitle("");
  xframe->GetXaxis()->SetTitle("m_{bb} [GeV]");
  xframe->GetYaxis()->SetTitle("Ratio");
  xframe->GetYaxis()->SetTitleOffset(1.2);
  xframe->Draw();
  canv->Update();
  canv->Print("tf_"+names[iCAT]+".png");

}
