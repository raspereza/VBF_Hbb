#include "HttStylesNew.cc"
// 0 - signal
// 1 - ZJets
// 2 - ggH
// 3 - Data
// selection = 0 (ggH), 1 (VBF), 2 (ZJets)

void PlotBDT(TString var = "bdtout_1",
	     int nbins = 20 , 
	     float xmin = 0,
	     float xmax = 1.0,
	     int selection = 2) {
  
  SetStyle();

  TFile * file = new TFile("ntuples/mbb_and_bdt_all_Nom.root");
  TNtuple * data_tuple = (TNtuple*)file->Get("Mass_and_BDT_DATA");
  TNtuple * ggH_tuple = (TNtuple*)file->Get("Mass_and_BDT_ggF_Hbb_POWHEG");
  TNtuple * qqH_tuple = (TNtuple*)file->Get("Mass_and_BDT_VBF_Hbb_Dipol");
  TNtuple * zj_tuple = (TNtuple*)file->Get("Mass_and_BDT_ZJets");

  TString cat("VBF");
  if (var=="bdtout_1") cat = "ZJ";
  if (var=="bdtout_2") cat = "ggH";
  if (var=="bdtout_3") cat = "data";

  TString Sel("ggH");


  TString cuts("mbb_reg>80&&mbb_reg<200");

  if (selection==1) {
    Sel = "VBF";
    cuts += "&&bdtout_2<0.1";    
  }

  if (selection==2) {
    Sel = "ZJets";
    cuts += "&&bdtout_2<0.1&&bdtout_0<0.6";    
  }


  TH1D * data_hist = new TH1D("data_hist","",nbins,xmin,xmax);
  TH1D * ggH_hist = new TH1D("ggH_hist","",nbins,xmin,xmax);
  TH1D * qqH_hist = new TH1D("qqH_hist","",nbins,xmin,xmax);
  TH1D * zj_hist = new TH1D("zj_hist","",nbins,xmin,xmax);
  
  TCanvas * dummy = new TCanvas("dummy","",500,500);
  data_tuple->Draw(var+">>data_hist","weight*("+cuts+")");
  ggH_tuple->Draw(var+">>ggH_hist","weight*("+cuts+")");
  qqH_tuple->Draw(var+">>qqH_hist","weight*("+cuts+")");
  zj_tuple->Draw(var+">>zj_hist","weight*("+cuts+")");
  delete dummy;

  float data_norm = data_hist->GetSumOfWeights();
  float ggH_norm  = ggH_hist->GetSumOfWeights();
  float qqH_norm  = qqH_hist->GetSumOfWeights();
  float zj_norm   = zj_hist->GetSumOfWeights();

  data_hist->Scale(1./data_norm);
  ggH_hist->Scale(1./ggH_norm);
  qqH_hist->Scale(1./qqH_norm);
  zj_hist->Scale(1./zj_norm);

  InitSignal(data_hist);
  InitSignal(ggH_hist);
  InitSignal(qqH_hist);
  InitSignal(zj_hist);

  data_hist->SetLineStyle(1);
  data_hist->SetLineWidth(3);
  data_hist->SetLineColor(1);
  data_hist->SetMarkerSize(0);

  ggH_hist->SetLineStyle(1);
  ggH_hist->SetLineWidth(3);
  ggH_hist->SetLineColor(2);
  ggH_hist->SetMarkerSize(0);

  qqH_hist->SetLineStyle(1);
  qqH_hist->SetLineWidth(3);
  qqH_hist->SetLineColor(3);
  qqH_hist->SetMarkerSize(0);

  zj_hist->SetLineStyle(1);
  zj_hist->SetLineWidth(3);
  zj_hist->SetLineColor(4);
  zj_hist->SetMarkerSize(0);

  float yMax = data_hist->GetMaximum();
  if (ggH_hist->GetMaximum()>yMax) yMax = ggH_hist->GetMaximum();
  if (qqH_hist->GetMaximum()>yMax) yMax = qqH_hist->GetMaximum();
  if (zj_hist->GetMaximum()>yMax) yMax = zj_hist->GetMaximum();

  data_hist->GetYaxis()->SetRangeUser(0.,1.2*yMax);
  data_hist->GetXaxis()->SetTitle(var);
  data_hist->GetXaxis()->SetTitleOffset(1.3);
  data_hist->GetYaxis()->SetTitleOffset(1.4);
  data_hist->GetYaxis()->SetTitle("normalised");

  TCanvas * canv = MakeCanvas("canv","",600,600);
  data_hist->Draw("h");
  ggH_hist->Draw("hsame");
  qqH_hist->Draw("hsame");
  zj_hist->Draw("hsame");
  TLegend * leg = new TLegend(0.6,0.55,0.85,0.85);
  SetLegendStyle(leg);
  leg->SetHeader(cat);
  leg->AddEntry(data_hist,"Data","l");
  leg->AddEntry(zj_hist,"Z+Jets","l");
  leg->AddEntry(qqH_hist,"VBF","l");
  leg->AddEntry(ggH_hist,"ggH","l");
  leg->Draw();
  canv->Update();
  canv->Print(var+"_"+Sel+".png");


}
