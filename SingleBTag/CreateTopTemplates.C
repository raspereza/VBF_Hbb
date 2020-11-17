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

  RooRealVar mean("mean_tt_"+names[iCAT],"Mean",125,80,200);
  RooRealVar sigma("sigma_tt_"+names[iCAT],"Width",50,0,50);
  RooRealVar alpha1("alpha1_tt_"+names[iCAT],"Alpha",2.0,0.5,10.);
  RooRealVar n1("n1_tt_"+names[iCAT],"n",4,3,20);
  RooRealVar alpha2("alpha2_tt_"+names[iCAT],"Alpha",1.0,0,10.);
  RooRealVar n2("n2_tt_"+names[iCAT],"n",2,0,20);

  RooRealVar b0("b0_tt_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_tt_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_tt_"+names[iCAT],"b2",0,1);

  b0.setConstant(false);
  b1.setConstant(false);
  b2.setConstant(false);

  RooRealVar ftt("ftt_"+names[iCAT],"ftt",0,1);

  RooRealVar mean_scale("CMS_tt_scale_mbb_13TeV_2018","Mbb_scale",1.0,0.5,1.5);
  RooRealVar sigma_res("CMS_tt_res_mbb_13TeV_2018","Mbb_scale",1.0,0.5,1.5);
    
  RooFormulaVar mean_shifted("mean_shifted_tt_"+names[iCAT],"@0*@1",RooArgList(mean,mean_scale));
  RooFormulaVar sigma_shifted("sigma_res_tt_"+names[iCAT],"@0*@1",RooArgList(sigma,sigma_res));

  RooBernstein BRN("brn_tt_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooDoubleCB dscbx("dscb_tt_"+names[iCAT],"DoubleCB",mbb,mean,sigma,alpha1,n1,alpha2,n2);
  RooAddPdf ttx("ttx","tt",RooArgList(dscbx,BRN),ftt);

  RooDataHist data("datatt","datatt",mbb,histopt);
  RooFitResult * res = ttx.fitTo(data,Save());

  RooDoubleCB dscb("dscb_tt_"+names[iCAT],"DoubleCB",mbb,mean_shifted,sigma_shifted,alpha1,n1,alpha2,n2);
  RooAddPdf tt("tt_"+names[iCAT],"tt",RooArgList(dscb,BRN),ftt);

  alpha1.setConstant(true);
  alpha2.setConstant(true);
  mean.setConstant(true);
  sigma.setConstant(true);
  n1.setConstant(true);
  n2.setConstant(true);
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
void CreateTopTemplates() {

  TFile * file = new TFile("/afs/cern.ch/work/m/mukherje/public/ForVBFHbb/mbb_and_bdt_all_BJETbtg.root");
  TNtuple * treett = (TNtuple*)file->Get("Mass_and_BDT_tt");

  TFile * fileOutput = new TFile("root_shape/tt_singleb_shapes.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","w");

  for (int i=0; i<5; ++i) 
    CreateTopPDF(i,treett,w);

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();

}
