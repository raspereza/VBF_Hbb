#include "HttStylesNew.cc"
#include "CMS_lumi.C"
#include "HtoH.h"
#include <map>
#include <vector>
//
void Plot(
	  TString MLFit_file = "/afs/cern.ch/work/r/rasp/public/forSoumya/mlfit_obs.root",
	  int nBins = 1500,
	  double xmin = 60,
	  double xmax = 210,
	  int nBinsNew = 30 // new number of bins 
	  ) {

  double binWidth = (xmax-xmin)/double(nBins);

  TH1::SetDefaultSumw2();

  bool logY = false;
  TString era("2018");    
  double frameScale = 0.0;
  double scaleYupper = 1.2;
  if (logY)
    scaleYupper = 5000.;

  SetStyle();
  //  gStyle->SetErrorX(0);

  TString xtitle("m_{bb} (GeV)");

  TString shapesDir("shapes_fit_s");
  lumi_13TeV = "2018, 54.4 fb^{-1}";

  std::vector<TString> channels = {
    "ch1","ch2","ch3","ch4","ch5","ch6","ch7","ch8"
  };

  TFile * mlfit = new TFile(MLFit_file);
  
  double width = (xmax-xmin)/double(nBinsNew);
  double xbins[100];
  for (int iB=0; iB<=nBinsNew; ++iB)
    xbins[iB] = xmin + double(iB)*width;


  double scaleHist = double(nBins)/double(nBinsNew);

  TH1D * dataNewHist = new TH1D("dataNewHist","",nBinsNew,xbins);
  TH1D * bkgNewHist  = new TH1D("bkgNewHist","",nBinsNew,xbins);
  TH1D * sigNewHist  = new TH1D("sigNewHist","",nBinsNew,xbins);
  TH1D * sbNewHist   = new TH1D("sbNewHist","",nBinsNew,xbins);

  TH1D * dataHist = new TH1D("dataHist","",nBins,xmin,xmax);
  TH1D * bkgHist  = new TH1D("bkgHist","",nBins,xmin,xmax);
  TH1D * sigHist  = new TH1D("sigHist","",nBins,xmin,xmax);
  TH1D * sbHist   = new TH1D("sbHist","",nBins,xmin,xmax);

  std::cout << std::endl;
  for (auto channel : channels ) {
    //    TString channel = channels.at(iC);
    TH1D * total_bkg = (TH1D*)mlfit->Get(shapesDir+"/"+channel+"/total_background");
    TH1D * total_sig = (TH1D*)mlfit->Get(shapesDir+"/"+channel+"/total_signal");
    TH1D * total_sb  = (TH1D*)mlfit->Get(shapesDir+"/"+channel+"/total");
    TGraphAsymmErrors * dataGraph = (TGraphAsymmErrors*)mlfit->Get("shapes_prefit/"+channel+"/data");
    double weight = total_sig->GetSumOfWeights()/total_sb->GetSumOfWeights();
    
    TH1D * total_bkg_hist = new TH1D("total_bkg_hist_"+channel,"",nBins,xmin,xmax);
    TH1D * total_sig_hist = new TH1D("total_sig_hist_"+channel,"",nBins,xmin,xmax);
    TH1D * total_sb_hist = new TH1D("total_sb_hist_"+channel,"",nBins,xmin,xmax);
    TH1D * total_data_hist = new TH1D("total_data_hist_"+channel,"",nBins,xmin,xmax);

    for (int iB=1; iB<=nBins; ++iB) {

      double dX = dataGraph->GetY()[iB-1]*binWidth;
      double dE = TMath::Sqrt(dX);

      double bX = total_bkg->GetBinContent(iB)*binWidth;
      double sX = total_sig->GetBinContent(iB)*binWidth;
      double sbX = total_sb->GetBinContent(iB)*binWidth;
      double bE  = total_bkg->GetBinError(iB)*binWidth;
      double sE  = total_sig->GetBinError(iB)*binWidth;
      double sbE = total_sb->GetBinError(iB)*binWidth;

      total_data_hist->SetBinContent(iB,dX);
      total_data_hist->SetBinError(iB,dE);

      total_bkg_hist->SetBinContent(iB,bX);
      total_bkg_hist->SetBinError(iB,bE);

      total_sig_hist->SetBinContent(iB,sX);
      total_sig_hist->SetBinError(iB,sE);

      total_sb_hist->SetBinContent(iB,sbX);
      total_sb_hist->SetBinError(iB,sbE);
    }

    TH1D * total_bkg_new_hist  = (TH1D*)TH1DtoTH1D(total_bkg_hist,nBinsNew,xbins,true,"new");
    TH1D * total_sig_new_hist  = (TH1D*)TH1DtoTH1D(total_sig_hist,nBinsNew,xbins,true,"new");
    TH1D * total_sb_new_hist   = (TH1D*)TH1DtoTH1D(total_sb_hist,nBinsNew,xbins,true,"new");
    TH1D * total_data_new_hist = (TH1D*)TH1DtoTH1D(total_data_hist,nBinsNew,xbins,true,"new");

    total_bkg_hist->Scale(weight*scaleHist);
    total_sig_hist->Scale(weight*scaleHist);
    total_sb_hist->Scale(weight*scaleHist);

    total_bkg_new_hist->Scale(weight);
    total_sig_new_hist->Scale(weight);
    total_sb_new_hist->Scale(weight);
    total_data_new_hist->Scale(weight);
    
    std::cout << channel << " : weight = " << weight << std::endl;
    for (int iB=1; iB<=nBinsNew; ++iB) {
      double xD = total_data_new_hist->GetBinContent(iB);
      double eD = total_data_new_hist->GetBinError(iB);
      double xB = total_bkg_new_hist->GetBinContent(iB);
      double eB = total_bkg_new_hist->GetBinError(iB);
      
      std::cout << "bin " << iB << "  :  data = " << xD << " +/- " << eD << std::endl; 
      //      std::cout << channel << ":" << iB << "  " << xB << " +/- " << eB << std::endl; 
      //      total_data_new_hist->SetBinContent(iB,weight*xD);    
      //      total_data_new_hist->SetBinError(iB,weight*eD);
    }
    std::cout << std::endl;
    
    bkgNewHist->Add(total_bkg_new_hist);
    sigNewHist->Add(total_sig_new_hist);
    sbNewHist->Add(total_sb_new_hist);
    dataNewHist->Add(total_data_new_hist);

    bkgHist->Add(total_bkg_hist);
    sigHist->Add(total_sig_hist);
    sbHist->Add(total_sb_hist);
    dataHist->Add(total_data_hist);

  }

  InitSignal(bkgNewHist);
  InitSignal(sbNewHist);

  InitSignal(bkgHist);
  InitSignal(sbHist);

  InitData(dataNewHist);
  InitData(dataHist);

  bkgNewHist->SetLineColor(4);
  bkgNewHist->SetLineWidth(2);
  bkgNewHist->SetLineStyle(1);

  sbNewHist->SetLineColor(2);
  sbNewHist->SetLineWidth(2);
  sbNewHist->SetLineStyle(1);

  bkgHist->SetLineColor(4);
  bkgHist->SetLineWidth(2);
  bkgHist->SetLineStyle(1);

  sbHist->SetLineColor(2);
  sbHist->SetLineWidth(2);
  sbHist->SetLineStyle(1);

  TH1D * bkgdErr = (TH1D*)bkgNewHist->Clone("bkgdErr");
  bkgdErr->SetFillStyle(3013);
  bkgdErr->SetFillColor(1);
  bkgdErr->SetMarkerStyle(21);
  bkgdErr->SetMarkerSize(0);  

  for (int iB=1; iB<=nBins; ++iB) {

    sbHist->SetBinError(iB,0.);
    bkgHist->SetBinError(iB,0.);
    sigHist->SetBinError(iB,0.);

  }

  std::cout << "weighted data distribution - > " << std::endl;
  for (int iB=1; iB<=nBinsNew; ++iB) {

    sbNewHist->SetBinError(iB,0.);
    bkgNewHist->SetBinError(iB,0.);
    sbNewHist->SetBinError(iB,0.);

    std::cout << "bin " << iB << " : data = " << dataNewHist->GetBinContent(iB) << " +/- " << dataNewHist->GetBinError(iB) << std::endl;

  }

  double frameYmin = 0;
  double frameYmax = scaleYupper*dataNewHist->GetMaximum();
  if (logY) 
    frameYmin = 1.0;

  dataNewHist->GetYaxis()->SetRangeUser(frameYmin,frameYmax);

  dataNewHist->GetXaxis()->SetLabelFont(42);
  dataNewHist->GetXaxis()->SetLabelOffset(0.04);
  dataNewHist->GetXaxis()->SetLabelSize(0.);
  dataNewHist->GetXaxis()->SetTickLength(0.04);
  dataNewHist->GetXaxis()->SetTitleOffset(1.2);
  dataNewHist->GetXaxis()->SetTitleSize(0.14);
  dataNewHist->GetXaxis()->SetTitle(xtitle);

  dataNewHist->GetYaxis()->SetLabelFont(42);
  dataNewHist->GetYaxis()->SetLabelSize(0.06);
  dataNewHist->GetYaxis()->SetTickLength(0.02);
  dataNewHist->GetYaxis()->SetLabelOffset(0.01);
  dataNewHist->GetYaxis()->SetTitleOffset(0.9);
  dataNewHist->GetYaxis()->SetTitleSize(0.08);
  dataNewHist->GetYaxis()->SetTitle("S/(S+B) weighted");


  TCanvas * canv1 = MakeCanvas("canv1", "", 600, 700);
  TPad * upper = new TPad("upper", "pad",0,0.41,1,1);
  upper->Draw();
  upper->cd();
  upper->SetFillColor(0);
  upper->SetBorderMode(0);
  upper->SetBorderSize(10);
  upper->SetTickx(1);
  upper->SetTicky(1);
  upper->SetLeftMargin(0.17);
  upper->SetRightMargin(0.05);
  upper->SetBottomMargin(0.02);
  upper->SetFrameFillStyle(0);
  upper->SetFrameLineStyle(0);
  upper->SetFrameLineWidth(2);
  upper->SetFrameBorderMode(0);
  upper->SetFrameBorderSize(10);
  upper->SetFrameFillStyle(0);
  upper->SetFrameLineStyle(0);
  upper->SetFrameLineWidth(2);
  upper->SetFrameBorderMode(0);
  upper->SetFrameBorderSize(10);

  dataNewHist->Draw("e1");
  sbHist->Draw("hsame");
  bkgHist->Draw("hsame");
  bkgdErr->Draw("e2same");
  dataNewHist->Draw("e1same");

  TLegend * leg  = new TLegend(0.65,0.45,0.90,0.7);
  SetLegendStyle(leg);
  leg->SetTextSize(0.06);
  //  leg->SetHeader(Header);
  leg->AddEntry(dataNewHist,"observed","lp");
  leg->AddEntry(sbHist,"s+b","l");
  leg->AddEntry(bkgHist,"b","l");
  leg->Draw();

  writeExtraText = true;
  extraText = "Preliminary";
  CMS_lumi(upper,4,33); 

  if (logY) upper->SetLogy(true);

  upper->Draw("SAME");
  upper->RedrawAxis();
  upper->Modified();
  upper->Update();
  canv1->cd();

  TH1D * ratioH = (TH1D*)dataNewHist->Clone("ratioH");
  TH1D * ratioErrH = (TH1D*)bkgdErr->Clone("ratioErrH");
  TH1D * ratioSBH = (TH1D*)sbHist->Clone("ratioSBH");
  ratioH->SetMarkerColor(1);
  ratioH->SetMarkerStyle(20);
  ratioH->SetMarkerSize(1.2);
  ratioH->SetLineColor(1);
  ratioH->GetYaxis()->SetNdivisions(505);

  ratioH->GetXaxis()->SetLabelFont(42);
  ratioH->GetXaxis()->SetLabelOffset(0.04);
  ratioH->GetXaxis()->SetLabelSize(0.12);
  ratioH->GetXaxis()->SetTickLength(0.05);
  ratioH->GetXaxis()->SetTitleOffset(1.2);
  ratioH->GetXaxis()->SetTitleSize(0.14);
  ratioH->GetXaxis()->SetTitle(xtitle);


  ratioH->GetYaxis()->SetLabelFont(42);
  ratioH->GetYaxis()->SetLabelSize(0.08);
  ratioH->GetYaxis()->SetTickLength(0.04);
  ratioH->GetYaxis()->SetLabelOffset(0.01);
  ratioH->GetYaxis()->SetTitleOffset(0.6);
  ratioH->GetYaxis()->SetTitleSize(0.12);
  ratioH->GetYaxis()->SetTitle("Data-Bkg");

  double ymax = -1.;
  double ymin =  1.;
  for (int iB=1; iB<=nBinsNew; ++iB) {
    double xNewData = dataNewHist->GetBinContent(iB);
    double xNewBkg  = bkgNewHist->GetBinContent(iB);
    double eNewData = dataNewHist->GetBinError(iB);
    ratioH->SetBinContent(iB,xNewData-xNewBkg);
    ratioH->SetBinError(iB,eNewData);
    double x = ratioH->GetBinContent(iB);
    if (x>ymax) ymax = x;
    if (x<ymin) ymin = x;
  }
  for (int iB=1; iB<=nBins; ++iB) {
    double xS = sigHist->GetBinContent(iB);
    ratioSBH->SetBinContent(iB,xS);
    ratioSBH->SetBinError(iB,0.);
    double eBkg = bkgHist->GetBinError(iB);
    ratioErrH->SetBinContent(iB,0.);
    ratioErrH->SetBinError(iB,eBkg);
  }

  std::cout << std::endl;
  std::cout << "(Data - Bkg) plot : " << std::endl;
  std::cout << "Xmin = " << ymin << std::endl;
  std::cout << "Xmax = " << ymax << std::endl;
  ratioH->GetYaxis()->SetRangeUser(1.2*ymin,1.2*ymax);


  // ------------>Primitives in pad: lower
  TPad * lower = new TPad("lower", "pad",0,0,1,0.40);
  lower->Draw();
  lower->cd();
  lower->SetFillColor(0);
  lower->SetBorderMode(0);
  lower->SetBorderSize(10);
  lower->SetGridy();
  lower->SetTickx(1);
  lower->SetTicky(1);
  lower->SetLeftMargin(0.17);
  lower->SetRightMargin(0.05);
  lower->SetTopMargin(0.026);
  lower->SetBottomMargin(0.35);
  lower->SetFrameFillStyle(0);
  lower->SetFrameLineStyle(0);
  lower->SetFrameLineWidth(2);
  lower->SetFrameBorderMode(0);
  lower->SetFrameBorderSize(10);
  lower->SetFrameFillStyle(0);
  lower->SetFrameLineStyle(0);
  lower->SetFrameLineWidth(2);
  lower->SetFrameBorderMode(0);
  lower->SetFrameBorderSize(10);

  ratioH->Draw("e1");
  ratioErrH->Draw("e2same");
  ratioSBH->Draw("hsame");

  lower->Modified();
  lower->RedrawAxis();
  canv1->cd();
  canv1->Modified();
  canv1->cd();
  canv1->SetSelected(canv1);
  canv1->Update();
  canv1->Print("Zbb.png");
  
}
