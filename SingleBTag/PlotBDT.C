#include "HttStylesNew.cc"
void PlotBDT() {

  int nl = 4;
  double xl[4] = {0.0,0.3,0.6,0.8};

  gStyle->SetOptStat(0000);

  TFile * file = new TFile("mbb_and_BDT.root");
  TNtuple * tree = (TNtuple*)file->Get("MinvB");
  TH1D * hist = new TH1D("data_BDT","",20,-1,1);

  TNtuple * treeGGH = (TNtuple*)file->Get("MinvS1");
  TNtuple * treeVBF = (TNtuple*)file->Get("MinvS0");
  TH1D * histGGH = new TH1D("ggh_BDT","",20,-1,1);
  TH1D * histVBF = new TH1D("vbf_BDT","",20,-1,1);

  TCanvas * dummy = new TCanvas("dummy","",500,500);
  tree->Draw("bdtout>>data_BDT","mbb>80&&mbb<200");
  treeGGH->Draw("bdtout>>ggh_BDT","weight*(mbb>80&&mbb<200)");
  treeVBF->Draw("bdtout>>vbf_BDT","weight*(mbb>80&&mbb<200)");
  delete dummy;

  for (int iB=1; iB<=50; ++iB) {
    histGGH->SetBinError(iB,0);
    histVBF->SetBinError(iB,0);
  }

  hist->GetYaxis()->SetTitle("Events / 0.1");
  hist->GetXaxis()->SetTitle("BDT");

  hist->GetYaxis()->SetRangeUser(1e-1,100*hist->GetMaximum());

  hist->SetLineColor(2);
  hist->SetLineWidth(2);
  
  histGGH->Add(histGGH,histVBF);
  histGGH->SetLineColor(4);
  histGGH->SetLineWidth(2);

  histVBF->SetLineColor(4);
  histVBF->SetLineStyle(2);
  histVBF->SetLineWidth(2);

  TCanvas * canv = MakeCanvas("canv","",700,700);

  hist->Draw();
  histGGH->Draw("hsame");
  histVBF->Draw("hsame");  

  TLegend * leg = new TLegend(0.6,0.7,0.9,0.9);  
  SetLegendStyle(leg);
  leg->SetHeader("single-b");
  leg->AddEntry(hist,"Data","l");
  leg->AddEntry(histGGH,"ggH","l");
  leg->AddEntry(histVBF,"qqH","l");
  leg->Draw();

  for (int il=0; il<nl; ++il) {
    TLine * line = new TLine(xl[il],0.1,xl[il],1e+4);
    line->SetLineStyle(3);
    line->SetLineWidth(2);
    line->Draw();

  }

  canv->SetLogy(true);
  canv->Update();
  canv->Print("singleb_BDT.png");


}
