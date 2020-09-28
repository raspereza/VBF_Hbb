#include "HttStylesNew.cc"
#include "Common.h"

void PlotSignalFits(int iCAT=4) {

  using namespace RooFit;

  TH1::SetDefaultSumw2(true);
  
  SetStyle();

  TFile * file = new TFile("mbb_and_BDT.root");
  
  TNtuple * treeGGH = (TNtuple*)file->Get("MinvS1");
  TNtuple * treeVBF = (TNtuple*)file->Get("MinvS0");


  TH1D * histGGH;
  TH1D * histVBF;

  Float_t yieldGGH;
  Float_t yieldVBF;
  TString nameGGHHist = "mbb_ggh";
  TString nameVBFHist = "mbb_vbf";
  histGGH = new TH1D(nameGGHHist,"",30,80,200);
  histVBF = new TH1D(nameVBFHist,"",30,80,200);
  TCanvas * dummy = new TCanvas("dummy","",800,700);
  treeGGH->Draw("mbb>>"+nameGGHHist,"weight*("+cuts[iCAT]+")");
  treeVBF->Draw("mbb>>"+nameVBFHist,"weight*("+cuts[iCAT]+")");
  histVBF->Add(histGGH,histVBF);
  delete dummy;

  InitData(histVBF);
  InitHist(histGGH,"","",kCyan,1001);
  for (int iB=1; iB<120; ++iB)
    histGGH->SetBinError(iB,0);


  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",80,200);

  RooRealVar mean("mean_sig_"+names[iCAT],"Mean",125,80,200);
  RooRealVar meanScale("CMS_mbb_scale","Mean",125,80,200);
  RooRealVar sigma("sigma_sig_"+names[iCAT],"Width",10,0,30);
  RooRealVar alpha("alpha_sig_"+names[iCAT],"Alpha",0.5,0,2.);
  RooRealVar n("n_sig_"+names[iCAT],"n",10,0,20);
    
  RooRealVar b0("b0_sig_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_sig_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_sig_"+names[iCAT],"b2",0,1);

  RooRealVar fsig("fsig_"+names[iCAT],"fsig",0,1);

  RooRealVar mean_scale("CMS_vbfbb_scale_mbb_selsingle_13TeV_2016","Mbb_scale",1.0,0.5,1.5);
  RooRealVar sigma_res("CMS_vbfbb_res_mbb_selsingle_13TeV_2016","Mbb_scale",1.0,0.5,1.5);
    
  RooFormulaVar mean_shifted("mean_shifted_sig","@0*@1",RooArgList(mean,mean_scale));
  RooFormulaVar sigma_shifted("sigma_res_sig","@0*@1",RooArgList(sigma,sigma_res));

  RooBernstein BRN("brn_sig_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cbx("cb_sig_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooAddPdf signalx("signalx","signal",RooArgList(cbx,BRN),fsig);

  RooDataHist data("dataGGH","dataGGH",mbb,histVBF);

  signalx.fitTo(data,Save());
  RooPlot * frame = mbb.frame();
  data.plotOn(frame);
  signalx.plotOn(frame);
  signalx.plotOn(frame,Components(BRN),LineStyle(kDashed));

  frame->SetTitle("              "+names[iCAT]);
  frame->GetYaxis()->SetTitle("Events / 2 GeV");
  frame->GetXaxis()->SetTitle("m_{bb} [GeV]");
  frame->GetYaxis()->SetTitleOffset(1.5);
  frame->GetYaxis()->SetRangeUser(0.,1.2*histVBF->GetMaximum());

  TCanvas * canv = new TCanvas("canv","",700,700);
  frame->Draw();
  histGGH->Draw("hsame");
  frame->Draw("same");
  TLegend * leg = new TLegend(0.6,0.65,0.8,0.8);
  SetLegendStyle(leg);
  leg->AddEntry(histVBF,"ggH+qqH","lp");
  leg->AddEntry(histGGH,"ggH","f");
  leg->Draw();
  canv->Update();
  canv->Print("signal_"+names[iCAT]+".png");
  

}
