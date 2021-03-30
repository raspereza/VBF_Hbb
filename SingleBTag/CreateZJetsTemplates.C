#include "Common.h"



using namespace RooFit;

void CreateTopPDF(int iCAT,
		     TNtuple * treezj,
		     RooWorkspace * w) {


  TH1::SetDefaultSumw2(true);

  TString namezjHist = "mbb_zj"+names[iCAT];
  TH1D * histopt = new TH1D(namezjHist,"",NbinsSig,xmin,xmax);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  treezj->Draw("mbb>>"+namezjHist,"weight*("+cuts[iCAT]+")");
  delete dummy;

  Float_t yieldzj = histopt->GetSumOfWeights();
  RooRealVar yield_zj("zj_yield_"+names[iCAT],"Yield",yieldzj,0.,2*yieldzj);
  

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  RooRealVar mean("mean_zj_"+names[iCAT],"Mean",90,80,200);
  RooRealVar sigma("sigma_zj_"+names[iCAT],"Width",50,0,50);
  RooRealVar alpha("alpha_zj_"+names[iCAT],"Alpha",2.0,0.5,10.);
  RooRealVar n("n_zj_"+names[iCAT],"n",2,0,20);
  RooRealVar b0("b0_zj_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_zj_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_zj_"+names[iCAT],"b2",0,1);

  b0.setConstant(false);
  b1.setConstant(false);
  b2.setConstant(false);

  RooRealVar fzj("fzj_"+names[iCAT],"fzj",0,1);

  RooBernstein BRN("brn_zj_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cbx("cb_zj_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooAddPdf zjx("zjx","zj",RooArgList(cbx,BRN),fzj);

  RooDataHist data("datazj","datazj",mbb,histopt);
  RooFitResult * res = zjx.fitTo(data,Save());

  RooCBShape cb("cb_zj_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooAddPdf zj("zj_"+names[iCAT],"zj",RooArgList(cb,BRN),fzj);

  alpha.setConstant(true);
  mean.setConstant(true);
  sigma.setConstant(true);
  n.setConstant(true);
  fzj.setConstant(true);
  b0.setConstant(true);
  b1.setConstant(true);
  b2.setConstant(true);

  cout << endl;
  cout << "+++++++++++++ " << names[iCAT] << "++++++++++ " << endl;
  cout << endl;
  b0.Print();
  b1.Print();
  b2.Print();
  cout  << "yields : zj = " << yieldzj << std::endl; 
  res->Print();

  w->import(zj);
  w->import(yield_zj);

}
void CreateZJetsTemplates() {

  TFile * file = new TFile("/afs/cern.ch/user/t/tumasyan/public/For_Soumya/Systematics_ROOTS/mbb_and_bdt_all_Nom_JEC.root");
  TNtuple * treezj = (TNtuple*)file->Get("Mass_and_BDT_ZJets");

  TFile * fileOutput = new TFile("root_shape/zjets_singleb_shapes.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","w");

  for (int i=0; i<5; ++i) 
    CreateTopPDF(i,treezj,w);

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
