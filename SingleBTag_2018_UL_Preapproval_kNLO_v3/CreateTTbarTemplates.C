#include "Common.h"

using namespace RooFit;

std::ofstream outtext("tt_out_full.txt");

void CreateTopPDF(int iCAT,
		  TNtuple * treett,
		  TNtuple * treest,
		  RooWorkspace * w) {

  TH1::SetDefaultSumw2(true);

  TString namettHist = "mbb_tt"+names[iCAT];
  TString namestHist = "mbb_st"+names[iCAT];
  TH1D * histopt = new TH1D(namettHist,"",NbinsSig,xmin,xmax);
  TH1D * histopt_st = new TH1D(namestHist,"",NbinsSig,xmin,xmax);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  treett->Draw("mbb_reg>>"+namettHist,"weight*("+cuts[iCAT]+")");
  treest->Draw("mbb_reg>>"+namestHist,"weight*("+cuts[iCAT]+")");
  histopt->Add(histopt_st);
  
  Float_t yieldtt = histopt->GetSumOfWeights();
  RooRealVar yield_tt("tt_"+names[iCAT]+"_norm","Yield",yieldtt,0.,2*yieldtt);

  delete histopt;
  delete histopt_st;

  histopt = new TH1D(namettHist,"",NbinsSig,xmin,xmax);
  histopt_st = new TH1D(namestHist,"",NbinsSig,xmin,xmax);
  treett->Draw("mbb_reg>>"+namettHist,"weight");
  treest->Draw("mbb_reg>>"+namestHist,"weight");
  histopt->Add(histopt_st);
  
  delete dummy;

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  RooRealVar mean("mean_tt_"+names[iCAT],"Mean",130,80,200);
  RooRealVar sigma("sigma_tt_"+names[iCAT],"Width",10,0,30);
  RooRealVar alpha("alpha_tt_"+names[iCAT],"Alpha",0.5,0.,2.);
  RooRealVar n("n_tt_"+names[iCAT],"n",10,0,20);
  RooRealVar b0("b0_tt_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_tt_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_tt_"+names[iCAT],"b2",0,1);
  RooRealVar ftt("f_tt_"+names[iCAT],"f_tt",0,1);

  RooBernstein BRN("brn_tt_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cb("cb_tt_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooAddPdf tt("tt_"+names[iCAT],"tt",RooArgList(cb,BRN),ftt);

  RooDataHist data("datatt","datatt",mbb,histopt);
  RooFitResult * res = tt.fitTo(data,Save(),SumW2Error(kTRUE));

  alpha.setConstant(true);
  mean.setConstant(true);
  sigma.setConstant(true);
  n.setConstant(true);
  ftt.setConstant(true);
  b0.setConstant(true);
  b1.setConstant(true);
  b2.setConstant(true);

  outtext << endl;
  outtext << "+++++++++++++ " << names[iCAT] << "++++++++++ " << endl;
  cout << endl;
  b0.Print();
  b1.Print();
  b2.Print();
  outtext  << "yields : tt = " << yieldtt << std::endl; 
  res->Print();

  w->import(tt);
  w->import(yield_tt);

}
void  CreateTTbarTemplates() {

  TFile * file = new TFile("ntuples/mbb_and_bdt_all_Nom.root");
  TNtuple * treett = (TNtuple*)file->Get("Mass_and_BDT_tt");
  TNtuple * treest = (TNtuple*)file->Get("Mass_and_BDT_Sandgle_Top");


  TFile * fileOutput = new TFile("root_shape/top_singleb_shapes_2018.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","w");

  for (int i=1; i<4; ++i) 
    CreateTopPDF(i,treett,treest,w);

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
