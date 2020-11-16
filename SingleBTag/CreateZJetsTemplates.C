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

  RooRealVar mean("mean_zj_"+names[iCAT],"Mean",125,80,200);
  RooRealVar sigma("sigma_zj_"+names[iCAT],"Width",50,0,50);
  RooRealVar alpha1("alpha1_zj_"+names[iCAT],"Alpha",2.0,0.5,10.);
  RooRealVar n1("n1_zj_"+names[iCAT],"n",4,3,20);
  RooRealVar alpha2("alpha2_zj_"+names[iCAT],"Alpha",1.0,0,10.);
  RooRealVar n2("n2_zj_"+names[iCAT],"n",2,0,20);

  RooRealVar b0("b0_zj_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_zj_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_zj_"+names[iCAT],"b2",0,1);

  b0.setConstant(false);
  b1.setConstant(false);
  b2.setConstant(false);

  RooRealVar fzj("fzj_"+names[iCAT],"fzj",0,1);

  RooRealVar mean_scale("CMS_zj_scale_mbb_13TeV_2018","Mbb_scale",1.0,0.5,1.5);
  RooRealVar sigma_res("CMS_zj_res_mbb_13TeV_2018","Mbb_scale",1.0,0.5,1.5);
    
  RooFormulaVar mean_shifted("mean_shifted_zj_"+names[iCAT],"@0*@1",RooArgList(mean,mean_scale));
  RooFormulaVar sigma_shifted("sigma_res_zj_"+names[iCAT],"@0*@1",RooArgList(sigma,sigma_res));

  RooBernstein BRN("brn_zj_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooDoubleCB dscbx("dscb_zj_"+names[iCAT],"DoubleCB",mbb,mean,sigma,alpha1,n1,alpha2,n2);
  RooAddPdf zjx("zjx","zj",RooArgList(dscbx,BRN),fzj);

  RooDataHist data("datazj","datazj",mbb,histopt);
  RooFitResult * res = zjx.fitTo(data,Save());

  RooDoubleCB dscb("dscb_zj_"+names[iCAT],"DoubleCB",mbb,mean_shifted,sigma_shifted,alpha1,n1,alpha2,n2);
  RooAddPdf zj("zj_"+names[iCAT],"zj",RooArgList(dscb,BRN),fzj);

  alpha1.setConstant(true);
  alpha2.setConstant(true);
  mean.setConstant(true);
  sigma.setConstant(true);
  n1.setConstant(true);
  n2.setConstant(true);
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

  TFile * file = new TFile("/afs/cern.ch/work/m/mukherje/public/ForVBFHbb/mbb_and_bdt_all_BJETbtg.root");
  TNtuple * treezj = (TNtuple*)file->Get("Mass_and_BDT_ZJets");

  TFile * fileOutput = new TFile("root_shape/zjets_singleb_shapes_14_11_2020.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","w");

  for (int i=0; i<5; ++i) 
    CreateTopPDF(i,treezj,w);

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();

}
