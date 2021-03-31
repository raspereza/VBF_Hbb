#include "Common.h"



using namespace RooFit;
void CreateTopPDF(int iCAT,
		     TNtuple * treett,
		     RooWorkspace * w) {


  TH1::SetDefaultSumw2(true);

  TString namettHist = "mbb_tt"+names[iCAT];
  TH1D * histopt = new TH1D(namettHist,"",NbinsSig,xmin,xmax);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  treett->Draw("mbb>>"+namettHist,"weight*("+cuts[iCAT]+")");
  delete dummy;

  Float_t yieldtt = histopt->GetSumOfWeights();
  RooRealVar yield_tt("tt_yield_"+names[iCAT],"Yield",yieldtt,0.,2*yieldtt);
  

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  RooRealVar mean("mean_tt_"+names[iCAT],"Mean",140,80,200);
  RooRealVar sigma("sigma_tt_"+names[iCAT],"Width",10,0,50);
  RooRealVar alpha("alpha_tt_"+names[iCAT],"Alpha",2.0,0.0,20.);
  RooRealVar n("n_tt_"+names[iCAT],"n",2,0,20);
  RooRealVar b0("b0_tt_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_tt_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_tt_"+names[iCAT],"b2",0,1);

  b0.setConstant(false);
  b1.setConstant(false);
  b2.setConstant(false);

  RooRealVar ftt("ftt_"+names[iCAT],"ftt",0,1);

  RooBernstein BRN("brn_tt_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cbx("cb_tt_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooAddPdf ttx("ttx","tt",RooArgList(cbx,BRN),ftt);

  RooDataHist data("datatt","datatt",mbb,histopt);
  RooFitResult * res = ttx.fitTo(data,Save());

  RooCBShape cb("cb_tt_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooAddPdf tt("tt_"+names[iCAT],"tt",RooArgList(cb,BRN),ftt);

  alpha.setConstant(true);
  mean.setConstant(true);
  sigma.setConstant(true);
  n.setConstant(true);
  ftt.setConstant(true);
  b0.setConstant(true);
  b1.setConstant(true);
  b2.setConstant(true);

  cout << endl;
  cout << "+++++++++++++ " << names[iCAT] << "++++++++++ " << endl;
  cout << endl;
  b0.Print();
  b1.Print();
  b2.Print();
  cout  << "yields : tt = " << yieldtt << std::endl; 
  res->Print();

  w->import(tt);
  w->import(yield_tt);
}
void CreateTTbarTemplates() {

  TFile * file = new TFile("mbb_and_bdt_all.root");
  TNtuple * treett = (TNtuple*)file->Get("Mass_and_BDT_tt");
  TFile * fileOutput = new TFile("root_shape/tt_doubleb_shapes.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","w");

  for (int i=0; i<4; ++i) 
    CreateTopPDF(i,treett,w);

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
