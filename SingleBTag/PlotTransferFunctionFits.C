#include "HttStylesNew.cc"
#include "Common.h"

void PlotTransferFunctionFits(int iCAT=4, 
			      int iORDER=2,
			      bool blind=true) {

  using namespace RooFit;

  SetStyle();

  TFile * file = new TFile("mbb_and_BDT.root");
  TNtuple * tree = (TNtuple*)file->Get("MinvO");
 
  TString Cuts = cuts[iCAT];
  TString Cuts0 = cuts[0];

  TH1::SetDefaultSumw2(true);
  TH1D * hist;
  TH1D * hist0; // lowest BDT bin 

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbbH";
  TString nameHist0 = "mbb0H";
  hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  hist0 = new TH1D(nameHist0,"",NbinsBkg,xmin,xmax);
  tree->Draw("mbb>>"+nameHist,Cuts);
  tree->Draw("mbb>>"+nameHist0,Cuts0);
  delete dummy;

  TH1D * ratio = DivideHist(hist,hist0,"ratioH");
  TH1D * ratioBlind = new TH1D("ratioBlindH","",NbinsBkg,xmin,xmax);
  for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
    double xcenter = ratio->GetBinCenter(iBin);
    if (xcenter>100.&&xcenter<150.) continue;
    ratioBlind->SetBinContent(iBin,ratio->GetBinContent(iBin));
    ratioBlind->SetBinError(iBin,ratio->GetBinError(iBin));
  }
  InitData(ratio);
  InitData(ratioBlind);
  ratio->SetMarkerSize(1.5);
  ratioBlind->SetMarkerSize(1.5);

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);
  RooRealVar a1("a1_"+names[iCAT],"a1",-4,4);
  RooRealVar a2("a2_"+names[iCAT],"a2",-4,4);
  RooRealVar a3("a3_"+names[iCAT],"a3",-4,4);
  RooRealVar a4("a4_"+names[iCAT],"a4",-4,4);

  RooArgList argList(a1);
  if (iORDER>=2)
    argList.add(a2);
  if (iORDER>=3)
    argList.add(a3);
  if (iORDER>=4)
    argList.add(a4);
  
  RooDataHist data("data_"+names[iCAT],"data",mbb,ratio);
  RooDataHist dataBlind("dataBlind_"+names[iCAT],"dataBlind",mbb,ratioBlind);

  std::string namePol = polynomNames.at(iORDER) + "_" + namesCAT.at(iCAT);
  RooGenericPdf Polynom(namePol.c_str(),genericPolynoms.at(iORDER).c_str(),RooArgSet(mbb,argList));    

  RooFitResult * fitRes = Polynom.fitTo(data,Save(),SumW2Error(kTRUE));  
  cout << endl;
  fitRes->Print();

  RooPlot* xframe = mbb.frame();
  data.plotOn(xframe,LineColor(0),MarkerColor(0));
  Polynom.plotOn(xframe);

  xframe->SetTitle("              "+names[iCAT]);
  xframe->GetYaxis()->SetTitle("Ratio");
  xframe->GetXaxis()->SetTitle("m_{bb} [GeV]");
  xframe->GetYaxis()->SetTitleOffset(1.6);

  RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",Polynom,data) ;
  double ndof = NbinsBkg - iORDER;
  double chi2 = chi2Roo.getVal();
  double prob = TMath::Prob(chi2,ndof);
  cout << "Chi2/ndof = " << chi2 << "/" << ndof << " = " << chi2/ndof << "  prob = " << prob << endl;
  cout << "HISTOGRAM ENTRIES = " << hist->GetSumOfWeights() << endl;
  char str1[20];
  sprintf(str1,"#chi^{2}/ndof = %4.2f",chi2/ndof);
  char str2[20];
  sprintf(str2,"P(#chi^{2}) = %4.2f",prob);
  
  
  TCanvas * canv = new TCanvas("canv","",700,700);
  xframe->Draw();
  if (blind)
    ratioBlind->Draw("e1same");
  else
    ratio->Draw("e1same");
  canv->Update();
  TLegend * leg = new TLegend(0.2,0.45,0.5,0.6);
  SetLegendStyle(leg);
  leg->AddEntry(hist,"Data ("+names[iCAT]+"/CAT4)","lp");
  leg->Draw();
  TLatex * text1 = new TLatex(0.25,0.35,str1);
  TLatex * text2 = new TLatex(0.25,0.25,str2);
  text1->SetNDC();
  text2->SetNDC();
  text1->SetTextSize(0.04);
  text2->SetTextSize(0.04);
  text1->Draw();
  text2->Draw();
  canv->Print("tf_"+names[iCAT]+".png");
  



}
