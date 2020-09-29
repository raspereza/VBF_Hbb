#include "HttStylesNew.cc"
#include "Common.h"
void PlotDataFits(int iCAT=4, 
		  int iORDER=3,
		  bool blind=true) {

  using namespace RooFit;

  SetStyle();

  TFile * file = new TFile("mbb_and_BDT.root");
  TNtuple * tree = (TNtuple*)file->Get("MinvO");

  TString cutsBlind;
  cutsBlind = cuts[iCAT] + "&&(mbb<100||mbb>150)";

  TH1::SetDefaultSumw2(true);
  TH1D * hist;
  TH1D * histBlind;

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_";
  TString nameBlindHist = "mbb_blind";
  hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  histBlind = new TH1D(nameBlindHist,"",NbinsBkg,xmin,xmax);
  tree->Draw("mbb>>"+nameHist,cuts[iCAT]);
  tree->Draw("mbb>>"+nameBlindHist,cutsBlind);
  delete dummy;
  InitData(hist);

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);
  RooRealVar b0("b0_"+names[iCAT],"b0",0,300);
  RooRealVar b1("b1_"+names[iCAT],"b1",0,300);
  RooRealVar b2("b2_"+names[iCAT],"b2",0,300);
  RooRealVar b3("b3_"+names[iCAT],"b3",0,300);
  RooRealVar b4("b4_"+names[iCAT],"b4",0,300);
  RooRealVar b5("b5_"+names[iCAT],"b5",0,300);

  RooArgList argList(b0,b1,b2);
  if (iORDER>=4)
    argList.add(b3);
  if (iORDER>=5)
    argList.add(b4);
  if (iORDER>=6)
    argList.add(b5);
  
  RooBernstein BRN("qcd_"+names[iCAT],"Bernstein",mbb,argList);
  RooDataHist data("data_"+names[iCAT],"data",mbb,hist);
  RooDataHist dataBlind("dataBlind","data",mbb,histBlind);

  RooFitResult * fitRes = BRN.fitTo(data,Save());  
  cout << endl;
  fitRes->Print();

  RooPlot* xframe = mbb.frame();
  if (blind)
    data.plotOn(xframe,LineColor(0),MarkerColor(0));
  else 
    data.plotOn(xframe);
  BRN.plotOn(xframe);
  if (blind)
    dataBlind.plotOn(xframe);

  xframe->SetTitle("              "+names[iCAT]);
  xframe->GetYaxis()->SetTitle("Events / 1 GeV");
  xframe->GetXaxis()->SetTitle("m_{bb} [GeV]");
  xframe->GetYaxis()->SetTitleOffset(1.6);

  RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",BRN,data) ;
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
  canv->Update();
  TLegend * leg = new TLegend(0.6,0.65,0.8,0.8);
  SetLegendStyle(leg);
  leg->AddEntry(hist,"Data","lp");
  leg->Draw();
  TLatex * text1 = new TLatex(0.25,0.35,str1);
  TLatex * text2 = new TLatex(0.25,0.25,str2);
  text1->SetNDC();
  text2->SetNDC();
  text1->SetTextSize(0.04);
  text2->SetTextSize(0.04);
  text1->Draw();
  text2->Draw();
  canv->Print("data_"+names[iCAT]+".png");
  



}
