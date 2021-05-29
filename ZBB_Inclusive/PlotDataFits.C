#include "HttStylesNew.cc"
#include "Common.h"
#include "CMS_lumi.C"
void PlotDataFits(int iCAT=3, 
		  int iORDER=4,
		  bool blind=true) {

  using namespace RooFit;

  double yMin = 1;
  double yMax = 10;

  TH1::SetDefaultSumw2(true);
  SetStyle();

  TFile * file = new TFile("/afs/cern.ch/user/m/mukherje/public/ForAlexei/Inclusive_doubleB/mbb_and_bdt_all_Nom_JER.root");
  
  TNtuple * tree = (TNtuple*)file->Get("Mass_and_BDT_DATA");
  TNtuple * tree_tt = (TNtuple*)file->Get("Mass_and_BDT_tt");
  TNtuple * tree_zj = (TNtuple*)file->Get("Mass_and_BDT_ZJets");
  TNtuple * tree_wj = (TNtuple*)file->Get("Mass_and_BDT_WJets");

  TString cutsBlind;
  cutsBlind = cuts[iCAT] + "&&(mbb<70||mbb>110)";


  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_";
  TString nameBlindHist = "mbb_blind";

  TH1D * hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  TH1D * qcdHist = new TH1D("qcdHist","",NbinsBkg,xmin,xmax);
  TH1D * histBlind = new TH1D(nameBlindHist,"",NbinsBkg,xmin,xmax);

  TH1D * hist_tt = new TH1D(nameHist+"_tt","",NbinsBkg,xmin,xmax);
  TH1D * histBlind_tt = new TH1D(nameBlindHist+"_tt","",NbinsBkg,xmin,xmax);

  TH1D * hist_zj = new TH1D(nameHist+"_zj","",NbinsBkg,xmin,xmax);
  TH1D * histBlind_zj = new TH1D(nameBlindHist+"_zj","",NbinsBkg,xmin,xmax);

  TH1D * hist_wj = new TH1D(nameHist+"_wj","",NbinsBkg,xmin,xmax);
  TH1D * histBlind_wj = new TH1D(nameBlindHist+"_wj","",NbinsBkg,xmin,xmax);

  tree->Draw("mbb>>"+nameHist,cuts[iCAT]);

  tree_tt->Draw("mbb>>"+nameHist+"_tt","weight*("+cuts[iCAT]+")");
  tree_zj->Draw("mbb>>"+nameHist+"_zj","weight*("+cuts[iCAT]+")");
  tree_wj->Draw("mbb>>"+nameHist+"_wj","weight*("+cuts[iCAT]+")");

  tree->Draw("mbb>>"+nameBlindHist,cutsBlind);
  tree_tt->Draw("mbb>>"+nameBlindHist+"_tt","weight*("+cutsBlind+")");
  tree_zj->Draw("mbb>>"+nameBlindHist+"_zj","weight*("+cutsBlind+")");
  tree_wj->Draw("mbb>>"+nameBlindHist+"_wj","weight*("+cutsBlind+")");

  hist->Add(hist_tt,-1.);
  hist->Add(hist_zj,-1.);
  hist->Add(hist_wj,-1.);

  histBlind->Add(histBlind_tt,-1.);
  histBlind->Add(histBlind_zj,-1.);
  histBlind->Add(histBlind_wj,-1.);

  yMax = 1.1 * hist->GetMaximum();

  delete dummy;
  InitData(hist);
  hist->SetMarkerSize(1.6);
  qcdHist->SetLineColor(4);
  qcdHist->SetLineWidth(3);

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);
  RooRealVar b0("b0_"+names[iCAT],"b0",-1.,1.);
  RooRealVar b1("b1_"+names[iCAT],"b1",-1.,1.);
  RooRealVar b2("b2_"+names[iCAT],"b2",-1.,1.);
  RooRealVar b3("b3_"+names[iCAT],"b3",-1.,1.);
  RooRealVar b4("b4_"+names[iCAT],"b4",-1.,1.);
  RooRealVar b5("b5_"+names[iCAT],"b5",-1.,1.);

  RooArgList argList(b0,b1);
  if (iORDER>=3)
    argList.add(b2);
  if (iORDER>=4)
    argList.add(b3);
  if (iORDER>=5)
    argList.add(b4);
  if (iORDER>=6)
    argList.add(b5);
  
  RooChebychev CHV("qcd_"+names[iCAT],"CHV",mbb,argList);
  RooDataHist data("data_"+names[iCAT],"data",mbb,hist);
  RooDataHist dataBlind("dataBlind","data",mbb,histBlind);

  RooFitResult * fitRes = CHV.fitTo(data,Save());  
  cout << endl;
  fitRes->Print();

  RooPlot* xframe = mbb.frame();
  if (blind)
    data.plotOn(xframe,LineColor(0),MarkerColor(0));
  else 
    data.plotOn(xframe);
  CHV.plotOn(xframe);
  if (blind)
    dataBlind.plotOn(xframe);

  xframe->SetTitle("");
  xframe->GetYaxis()->SetTitle("Events / 1 GeV");
  xframe->GetXaxis()->SetTitle("m_{bb} [GeV]");
  xframe->GetYaxis()->SetTitleOffset(1.8);
  xframe->GetYaxis()->SetRangeUser(yMin,yMax);

  RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",CHV,data) ;
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
  TLegend * leg = new TLegend(0.65,0.57,0.85,0.82);
  SetLegendStyle(leg);
  leg->SetTextSize(0.045);
  leg->SetHeader(names[iCAT]);
  leg->AddEntry(hist,"Data","lp");
  leg->AddEntry(qcdHist,"QCD model","l");
  leg->Draw();
  TLatex * text1 = new TLatex(0.25,0.35,str1);
  TLatex * text2 = new TLatex(0.25,0.25,str2);
  text1->SetNDC();
  text2->SetNDC();
  text1->SetTextSize(0.04);
  text2->SetTextSize(0.04);
  text1->Draw();
  text2->Draw();
  writeExtraText = true;
  extraText = "Preliminary";
  CMS_lumi(canv,4,33); 
  canv->Print("data_"+names[iCAT]+".png");
  



}
