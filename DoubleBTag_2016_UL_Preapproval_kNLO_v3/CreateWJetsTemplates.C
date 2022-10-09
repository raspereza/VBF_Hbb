#include "Common.h"

using namespace RooFit;

std::ofstream outtext("wj_out_full.txt");

void CreateWPDF(int iCAT,
		 TNtuple * tree,
		 RooWorkspace * w) {

  TH1::SetDefaultSumw2(true);

  TString nameHist = "mbb_wj"+names[iCAT];
  TH1D * hist = new TH1D(nameHist,"",NbinsSig,xmin,xmax);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  tree->Draw("mbb_reg>>"+nameHist,"weight*("+cuts[iCAT]+")");
  
  Float_t yieldwj = hist->GetSumOfWeights();
  RooRealVar yield_wj("wj_"+names[iCAT]+"_norm","Yield",yieldwj,0.,2*yieldwj);

  delete hist;
  hist = new TH1D(nameHist,"",Nbins_wj,xmin_wj,xmax_wj);
  tree->Draw("mbb_reg>>"+nameHist,"weight");
  
  delete dummy;

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);
  RooRealVar mbb_wj("mbb_wj_"+names[iCAT],"mass(bb)",xmin_wj,xmax_wj);

  RooRealVar mean("mean_wj_"+names[iCAT],"Mean",80,60,200);
  RooRealVar sigma("sigma_wj_"+names[iCAT],"Width",10,0,30);
  RooRealVar alpha("alpha_wj_"+names[iCAT],"Alpha",0.5,0.,2.);
  RooRealVar n("n_wj_"+names[iCAT],"n",10,0,20);
  RooRealVar b0("b0_wj_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_wj_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_wj_"+names[iCAT],"b2",0,1);
  RooRealVar f("f_wj_"+names[iCAT],"f",0,1);

  RooBernstein BRNx("brnx","Bernstein",mbb_wj,RooArgList(b0,b1,b2));
  RooCBShape cbx("cbx","CBshape",mbb_wj,mean,sigma,alpha,n);
  RooAddPdf wjx("wjx","wjx",RooArgList(cbx,BRNx),f);

  RooDataHist data("data","data",mbb_wj,hist);
  RooFitResult * res = wjx.fitTo(data,Save(),SumW2Error(kTRUE));

  alpha.setConstant(true);
  mean.setConstant(true);
  sigma.setConstant(true);
  n.setConstant(true);
  f.setConstant(true);
  b0.setConstant(true);
  b1.setConstant(true);
  b2.setConstant(true);

  RooBernstein BRN("brn_wj_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cb("cb_wj_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooAddPdf wj("wj_"+names[iCAT],"wj",RooArgList(cb,BRN),f);

  outtext << endl;
  outtext << "+++++++++++++ " << names[iCAT] << "++++++++++ " << endl;
  std::cout << endl;
  b0.Print();
  b1.Print();
  b2.Print();
  outtext  << "yields : wj = " << yieldwj << std::endl; 
  res->Print();

  w->import(wj);
  w->import(yield_wj);

}
void  CreateWJetsTemplates() {

  TFile * file = new TFile("ntuples/mbb_and_bdt_all_Nom.root");
  TNtuple * tree = (TNtuple*)file->Get("Mass_and_BDT_WJets");

  TFile * fileOutput = new TFile("root_shape/wj_doubleb_shapes_2016.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","w");

  for (int i=0; i<6; ++i) 
    CreateWPDF(i,tree,w);

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
