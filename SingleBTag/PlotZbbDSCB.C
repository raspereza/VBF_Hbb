#include "HttStylesNew.cc"
#include "Common.h"

void PlotZbbDSCB(int iCAT=0) {

  using namespace RooFit;

  TH1::SetDefaultSumw2(true);
  
  SetStyle();

  TFile * file = new TFile("/afs/cern.ch/user/t/tumasyan/public/ToBeChecked/mbb_and_bdt_all_BJETbtg.root");
  
  TNtuple * treezbb = (TNtuple*)file->Get("Mass_and_BDT_ZJets");


  TH1D * histzbb;

  Float_t yieldzbb;
  
  TString namezbbhist = "mbb_zbb";

  histzbb = new TH1D(namezbbhist,"",NbinsSig,xmin,xmax);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  treezbb->Draw("mbb>>"+namezbbhist,"weight*("+cuts[iCAT]+")");

  delete dummy;

  InitData(histzbb);
  InitHist(histzbb,"","",kGreen,1001);

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  RooRealVar mean("mean_sig_"+names[iCAT],"Mean",125,80,200);
  RooRealVar meanScale("CMS_mbb_scale","Mean",125,80,200);
  RooRealVar sigma("sigma_sig_"+names[iCAT],"Width",10,0,30);
  RooRealVar alpha1("alpha1_sig_"+names[iCAT],"Alpha",2.0,0.5,10.);
  RooRealVar n1("n1_sig_"+names[iCAT],"n",4,3,20);
  RooRealVar alpha2("alpha2_sig_"+names[iCAT],"Alpha",1.0,0,10.);
  RooRealVar n2("n2_sig_"+names[iCAT],"n",2,0,20);
    
  RooRealVar b0("b0_sig_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_sig_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_sig_"+names[iCAT],"b2",0,1);

  RooRealVar fsig("fsig_"+names[iCAT],"fsig",0,1);

  RooRealVar mean_scale("CMS_vbfbb_scale_mbb_selsingle_13TeV_2018","Mbb_scale",1.0,0.5,1.5);
  RooRealVar sigma_res("CMS_vbfbb_res_mbb_selsingle_13TeV_2018","Mbb_scale",1.0,0.5,1.5);
    
  RooFormulaVar mean_shifted("mean_shifted_sig","@0*@1",RooArgList(mean,mean_scale));
  RooFormulaVar sigma_shifted("sigma_res_sig","@0*@1",RooArgList(sigma,sigma_res));

  RooBernstein BRN("brn_sig_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooDoubleCB dscb("dscb_sig_"+names[iCAT],"DSCBshape",mbb,mean,sigma,alpha1,n1,alpha2,n2);
  RooAddPdf signalx("signalx","signal",RooArgList(dscb),fsig);

  RooDataHist data("dataGGH","dataGGH",mbb,histzbb);

  signalx.fitTo(data,Save());
  RooPlot * frame = mbb.frame();
  data.plotOn(frame);
  signalx.plotOn(frame);
  signalx.plotOn(frame,Components(BRN),LineStyle(kDashed));

  frame->SetTitle("              "+names[iCAT]);
  frame->GetYaxis()->SetTitle("Events / 5 GeV");
  frame->GetXaxis()->SetTitle("m_{bb} [GeV]");
  frame->GetYaxis()->SetTitleOffset(1.5);
  frame->GetYaxis()->SetRangeUser(0.,1.2*histzbb->GetMaximum()); 
  TCanvas * canv = new TCanvas("canv","",700,700);
  frame->Draw();
  histzbb->Draw("hsame"); 
  frame->Draw("same");
  TLegend * leg = new TLegend(0.6,0.65,0.8,0.8);
  SetLegendStyle(leg);
  leg->AddEntry(frame->FindObject("dscb"),"Fit", "L");
  leg->AddEntry(histzbb,"Zbb","F");
  leg->Draw();
  canv->Update();
  canv->Print("VBFHbb_meeting_20_10_2020/WJetsToBB_FullRegion_"+names[iCAT]+"DSCB.png");
  

}
