#include "Common.h"



using namespace RooFit;

void CreateSignalPDF(int iCAT,
		     TNtuple * treeGGH,
		     TNtuple * treeVBF,
		     RooWorkspace * w) {

  TH1::SetDefaultSumw2(true);

  TString nameGGHHist = "mbb_ggh"+names[iCAT];
  TString nameVBFHist = "mbb_vbf"+names[iCAT];
  TH1D * histGGH = new TH1D(nameGGHHist,"",NbinsSig,xmin,xmax);
  TH1D * histVBF = new TH1D(nameVBFHist,"",NbinsSig,xmin,xmax);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  treeGGH->Draw("mbb>>"+nameGGHHist,"weight*("+cuts[iCAT]+")");
  treeVBF->Draw("mbb>>"+nameVBFHist,"weight*("+cuts[iCAT]+")");

  Float_t yieldGGH = histGGH->GetSumOfWeights();
  Float_t yieldVBF = histVBF->GetSumOfWeights();
  RooRealVar yield_GGH("ggH_"+names[iCAT]+"_norm","Yield",yieldGGH,0.,2*yieldGGH);
  RooRealVar yield_VBF("qqH_"+names[iCAT]+"_norm","Yield",yieldVBF,0.,2*yieldVBF);

  delete histGGH;
  delete histVBF;

  histGGH = new TH1D(nameGGHHist,"",NbinsSig,xmin,xmax);
  histVBF = new TH1D(nameVBFHist,"",NbinsSig,xmin,xmax);
  treeGGH->Draw("mbb>>"+nameGGHHist,"weight");
  treeVBF->Draw("mbb>>"+nameVBFHist,"weight");
  delete dummy;


  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  RooRealVar mean("mean_sig_"+names[iCAT],"Mean",125,80,200);
  RooRealVar sigma("sigma_sig_"+names[iCAT],"Width",10,0,30);
  RooRealVar alpha("alpha_sig_"+names[iCAT],"Alpha",0.5,0,10.);
  RooRealVar n("n_sig_"+names[iCAT],"n",10,0,20);
    
  RooRealVar b0("b0_sig_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_sig_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_sig_"+names[iCAT],"b2",0,1);
  b0.setConstant(false);
  b1.setConstant(false);
  b2.setConstant(false);

  RooRealVar fsig("fsig_"+names[iCAT],"fsig",0,1);

  RooBernstein BRN("brn_sig_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cbx("cb_sig_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooAddPdf signalx("signalx","signal",RooArgList(cbx,BRN),fsig);

  histVBF->Add(histGGH,histVBF);
  RooDataHist data("dataGGH","dataGGH",mbb,histVBF);
  RooFitResult * res = signalx.fitTo(data,Save());

  RooBernstein BRN_qqH("brn_qqH_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooBernstein BRN_ggH("brn_ggH_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cb_qqH("cb_qqH_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooCBShape cb_ggH("cb_ggH_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooAddPdf qqH_signal("qqH_"+names[iCAT],"signal",RooArgList(cb_qqH,BRN_qqH),fsig);
  RooAddPdf ggH_signal("ggH_"+names[iCAT],"signal",RooArgList(cb_ggH,BRN_ggH),fsig);

  alpha.setConstant(true);
  mean.setConstant(true);
  sigma.setConstant(true);
  n.setConstant(true);
  fsig.setConstant(true);
  b0.setConstant(true);
  b1.setConstant(true);
  b2.setConstant(true);

  cout << endl;
  cout << "+++++++++++++ " << names[iCAT] << "++++++++++ " << endl;
  cout << endl;
  b0.Print();
  b1.Print();
  b2.Print();
  cout << "yields : ggH = " << yieldGGH 
       << "   qqH = " << yieldVBF
       << "   total = " << yieldGGH+yieldVBF << std::endl;
  res->Print();

  w->import(qqH_signal);
  w->import(ggH_signal);
  w->import(yield_GGH);
  w->import(yield_VBF);

}

void CreateSignalTemplates() {

  TFile * file = new TFile("/afs/cern.ch/user/t/tumasyan/public/For_Soumya/ggfPHG_mbb_and_bdt_all_.root");
  TNtuple * treeGGH = (TNtuple*)file->Get("Mass_and_BDT_ggF_Hbb");
  TNtuple * treeVBF = (TNtuple*)file->Get("Mass_and_BDT_VBF_Hbb");

  TFile * fileOutput = new TFile("root_shape/signal_singleb_shapes.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","signal");

  for (int i=0; i<5; ++i) 
    CreateSignalPDF(i,treeGGH,treeVBF,w);

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();

}
