#include "HttStylesNew.cc"
#include "Common.h"
void PlotDataGenericFits(int iCAT=0, 
			 int nB = 120,
			 bool blind = false) {

  using namespace RooFit;

  SetStyle();

  TFile * file = new TFile("Full_Data_stat.root");
  TTree * tree = (TTree*)file->Get("Stat");

  TString cutsBlind;
  cutsBlind = cuts[iCAT] + "&&(mbb<100||mbb>150)";

  TH1::SetDefaultSumw2(true);
  TH1D * hist;
  TH1D * histBlind;

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_";
  TString nameBlindHist = "mbb_blind";
  hist = new TH1D(nameHist,"",nB,80,200);
  histBlind = new TH1D(nameBlindHist,"",nB,80,200);
  tree->Draw("mbb>>"+nameHist,cuts[iCAT]);
  tree->Draw("mbb>>"+nameBlindHist,cutsBlind);
  delete dummy;
  InitData(hist);

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",80,200);
  RooRealVar b0("b0_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_"+names[iCAT],"b2",0,1);  
  RooRealVar b3("b3_"+names[iCAT],"b3",0,1);  
  RooRealVar b4("b4_"+names[iCAT],"b4",0,1);  

  RooGenericPdf BRN("qcd_"+names[iCAT],"(TMath::Power(@0,@1)+@3*TMath::Power(@0,@2))*TMath::Exp(-@4*TMath::Power(@0,@5))",RooArgList(mbb,b0,b1,b2,b3,b4));
  RooDataHist data("data_"+names[iCAT],"data",mbb,hist);
  RooDataHist dataBlind("dataBlind","data",mbb,histBlind);

  RooFitResult * fitRes = BRN.fitTo(data);  
  RooPlot* xframe = mbb.frame();
  if (blind)
    data.plotOn(xframe,LineColor(0),MarkerColor(0));
  else 
    data.plotOn(xframe);
  BRN.plotOn(xframe);
  if (blind)
    dataBlind.plotOn(xframe);

  xframe->SetTitle("              "+names[iCAT]);
  xframe->GetYaxis()->SetTitle("Events / 2 GeV");
  xframe->GetXaxis()->SetTitle("m_{bb} [GeV]");
  xframe->GetYaxis()->SetTitleOffset(1.6);

  TCanvas * canv = new TCanvas("canv","",700,700);
  xframe->Draw();
  canv->Update();
  TLegend * leg = new TLegend(0.6,0.65,0.8,0.8);
  SetLegendStyle(leg);
  leg->AddEntry(hist,"Data","lp");
  leg->Draw();
  canv->Print("data_"+names[iCAT]+".png");

  RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",BRN,data) ;
  double ndof = nB - 3;
  double chi2 = chi2Roo.getVal();
  double prob = TMath::Prob(chi2,ndof);
  cout << "Chi2/ndof = " << chi2 << "/" << ndof << " = " << chi2/ndof << "  prob = " << prob << endl;

}
