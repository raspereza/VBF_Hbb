#include "HttStylesNew.cc"
#include "CMS_lumi.C"
#include "HtoH.h"
#include <map>
#include <vector>
//
void Plot_channel(
		  TString MLFit_file = "./Fits/mlfit.root",
		  int nBins = 1200,
		  int nBinsNew = 24,
		  double xmin =  80,
		  double xmax = 200,
		  bool sbFit = true
		  ) {


  std::map<TString, TString> cat_channel = {
    
    {"DoubleB1_2016","ch1_ch1"},
    {"DoubleB2_2016","ch1_ch2"},
    {"DoubleB3_2016","ch1_ch3"},
    {"DoubleB4_2016","ch1_ch4"},
    {"DoubleB5_2016","ch1_ch5"},
    {"DoubleB6_2016","ch1_ch6"},

    {"DoubleB1_2018","ch2_ch1"},
    {"DoubleB2_2018","ch2_ch2"},
    {"DoubleB3_2018","ch2_ch3"},
    {"DoubleB4_2018","ch2_ch4"},
    {"DoubleB5_2018","ch2_ch5"},
    {"DoubleB6_2018","ch2_ch6"},
    
    {"SingleB1_2016","ch3_ch1"},
    {"SingleB2_2016","ch3_ch2"},
    {"SingleB3_2016","ch3_ch3"},

    {"SingleB1_2018","ch4_ch1"},
    {"SingleB2_2018","ch4_ch2"},
    {"SingleB3_2018","ch4_ch3"},
   
  };

  std::map<TString, double> cat_scale = {
    {"DoubleB1_2016",1.},
    {"DoubleB2_2016",1.},
    {"DoubleB3_2016",1.},
    {"DoubleB4_2016",1.},
    {"DoubleB5_2016",1.},
    {"DoubleB6_2016",1.},

    {"DoubleB1_2018",1.},
    {"DoubleB2_2018",1.},
    {"DoubleB3_2018",1.},
    {"DoubleB4_2018",1.},
    {"DoubleB5_2018",1.},
    {"DoubleB6_2018",1.},
    
    {"SingleB1_2016",1.},
    {"SingleB2_2016",1.},
    {"SingleB3_2016",1.},

    {"SingleB1_2018",1.},
    {"SingleB2_2018",1.},
    {"SingleB3_2018",1.},
  };


  gROOT->SetBatch();

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

  TString shapesDir("shapes_fit_b");
  if (sbFit) shapesDir = "shapes_fit_s";
  lumi_13TeV = "Run 2, 91 fb^{-1}";

  double scaleHist = double(nBins)/double(nBinsNew);

  double width = (xmax-xmin)/double(nBinsNew);
  double xbins[200];
  for (int iB=0; iB<=nBinsNew; ++iB) {
    xbins[iB] = xmin + double(iB)*width;      
  }
  
  TH1D * data_weighted_hist = new TH1D("data_weighted_hist","",nBinsNew,xbins);
  TH1D * bkg_weighted_hist  = new TH1D("bkg_weighted_hist","",nBinsNew,xbins);
  TH1D * qcd_weighted_hist  = new TH1D("qcd_weighted_hist","",nBinsNew,xbins);
  TH1D * sig_weighted_hist  = new TH1D("sig_weighted_hist","",nBinsNew,xbins);
  TH1D * zj_weighted_hist   = new TH1D("zj_weighted_hist","",nBinsNew,xbins);
  TH1D * sb_weighted_hist   = new TH1D("sb_weighted_hist","",nBinsNew,xbins);

  TFile * mlfit = new TFile(MLFit_file);
  for (auto map_cat : cat_channel ) {
    TString cat = map_cat.first;
    TString channel = map_cat.second;

    //  return;
    //    TString channel = channels.at(iC);
    TH1D * bkg_h   = (TH1D*)mlfit->Get(shapesDir+"/"+channel+"/total_background");
    TH1D * zj_h    = (TH1D*)mlfit->Get(shapesDir+"/"+channel+"/zj");
    TH1D * qcd_h   = (TH1D*)mlfit->Get(shapesDir+"/"+channel+"/qcd");
    TH1D * sig_h   = (TH1D*)mlfit->Get(shapesDir+"/"+channel+"/total_signal");
    TH1D * sb_h    = (TH1D*)mlfit->Get(shapesDir+"/"+channel+"/total");  
    TGraphAsymmErrors * dataGraph = (TGraphAsymmErrors*)mlfit->Get(shapesDir+"/"+channel+"/data");

    TH1D * total_data_hist = new TH1D("total_data_hist_"+channel,"",nBins,xmin,xmax);
    TH1D * total_bkg_hist  = new TH1D("total_bkg_hist_"+channel,"",nBins,xmin,xmax);
    TH1D * total_zj_hist   = new TH1D("total_zj_hist_"+channel,"",nBins,xmin,xmax);
    TH1D * total_qcd_hist  = new TH1D("total_qcd_hist_"+channel,"",nBins,xmin,xmax);
    TH1D * total_sig_hist  = new TH1D("total_sig_hist_"+channel,"",nBins,xmin,xmax);
    TH1D * total_sb_hist  = new TH1D("total_sb_hist_"+channel,"",nBins,xmin,xmax);

    for (int iB=1; iB<=nBins; ++iB) {

      double dX = dataGraph->GetY()[iB-1]*binWidth;
      double dE = TMath::Sqrt(dX);

      double bX  = bkg_h->GetBinContent(iB)*binWidth;
      double bE  = bkg_h->GetBinError(iB)*binWidth;

      double sX = sig_h->GetBinContent(iB)*binWidth;
      double sE = sig_h->GetBinError(iB)*binWidth;

      double sbX = sb_h->GetBinContent(iB)*binWidth;
      double sbE = sb_h->GetBinError(iB)*binWidth;

      double zX = zj_h->GetBinContent(iB)*binWidth;
      double zE = zj_h->GetBinError(iB)+binWidth;

      double qcdX  = qcd_h->GetBinContent(iB)*binWidth;
      double qcdE  = qcd_h->GetBinError(iB)*binWidth;

      total_data_hist->SetBinContent(iB,dX);
      total_data_hist->SetBinError(iB,dE);

      total_bkg_hist->SetBinContent(iB,bX);
      total_bkg_hist->SetBinError(iB,bE);

      total_sig_hist->SetBinContent(iB,sX);
      total_sig_hist->SetBinError(iB,sE);

      total_sb_hist->SetBinContent(iB,sbX);
      total_sb_hist->SetBinError(iB,sbE);

      total_zj_hist->SetBinContent(iB,zX);
      total_zj_hist->SetBinError(iB,zE);

      total_qcd_hist->SetBinContent(iB,qcdX);
      total_qcd_hist->SetBinError(iB,qcdE);

    }

    double signalX = total_sig_hist->GetSumOfWeights();
    double backgroundX = total_bkg_hist->GetSumOfWeights();
    double weight = signalX/(signalX+backgroundX);
    double signalRangeX = 0;
    double backgroundRangeX = 0;
    for (int ib=1; ib<=nBins; ++ib) {
      double lowEdge = total_sig_hist->GetBinLowEdge(ib);
      double highEdge = total_sig_hist->GetBinLowEdge(ib+1);
      if (lowEdge>100.&&highEdge<150.) {
	signalRangeX += total_sig_hist->GetBinContent(ib);
	backgroundRangeX += total_bkg_hist->GetBinContent(ib);
      }
    }

    std::cout << std::endl;
    std::cout << "Category " << cat << std::endl;
    std::cout << "Total signal     = " << signalX << " : " << signalRangeX << std::endl;
    std::cout << "Total background = " << backgroundX << ": " << backgroundRangeX << std::endl;
    std::cout << std::endl;

    TH1D * total_bkg_new_hist  = (TH1D*)TH1DtoTH1D(total_bkg_hist,nBinsNew,xbins,true,"new");
    TH1D * total_sig_new_hist  = (TH1D*)TH1DtoTH1D(total_sig_hist,nBinsNew,xbins,true,"new");
    TH1D * total_sb_new_hist   = (TH1D*)TH1DtoTH1D(total_sb_hist,nBinsNew,xbins,true,"new");
    TH1D * total_data_new_hist = (TH1D*)TH1DtoTH1D(total_data_hist,nBinsNew,xbins,true,"new");
    TH1D * total_zj_new_hist   = (TH1D*)TH1DtoTH1D(total_zj_hist,nBinsNew,xbins,true,"new");
    TH1D * total_qcd_new_hist  = (TH1D*)TH1DtoTH1D(total_qcd_hist,nBinsNew,xbins,true,"new");

    total_sig_hist->Scale(scaleHist);
    total_sb_hist->Scale(scaleHist);
    total_zj_hist->Scale(scaleHist);
    total_qcd_hist->Scale(scaleHist);
    total_bkg_hist->Scale(scaleHist);

    for (int ib=1; ib<=nBinsNew; ++ib) {
      double x_qcd = qcd_weighted_hist->GetBinContent(ib) + weight*total_qcd_new_hist->GetBinContent(ib);
      double error = weight*total_qcd_new_hist->GetBinError(ib);
      double e_qcd = qcd_weighted_hist->GetBinError(ib) + error;
      qcd_weighted_hist->SetBinContent(ib,x_qcd);
      qcd_weighted_hist->SetBinError(ib,e_qcd);
    }

    data_weighted_hist->Add(data_weighted_hist,total_data_new_hist,1.,weight);
    zj_weighted_hist->Add(zj_weighted_hist,total_zj_new_hist,1.,weight);
    sig_weighted_hist->Add(sig_weighted_hist,total_sig_new_hist,1.,weight);
    sb_weighted_hist->Add(sb_weighted_hist,total_sb_new_hist,1.,weight);
    bkg_weighted_hist->Add(bkg_weighted_hist,total_bkg_new_hist,1.,weight);

    InitData(total_data_new_hist);

    total_data_new_hist->SetMarkerSize(0.8);
    //    total_data_new_hist->SetErrorX(0.);


    InitSignal(total_qcd_hist);
    InitSignal(total_zj_hist);
    InitSignal(total_bkg_hist);
    InitSignal(total_sig_hist);
    InitSignal(total_sb_hist);

    total_qcd_hist->SetLineColor(4);
    total_qcd_hist->SetLineWidth(2);
    total_qcd_hist->SetLineStyle(1);
    
    total_bkg_hist->SetLineColor(2);
    total_bkg_hist->SetLineWidth(2);
    total_bkg_hist->SetLineStyle(1);
    
    total_zj_hist->SetLineColor(2);
    total_zj_hist->SetLineWidth(2);
    total_zj_hist->SetLineStyle(1);

    total_sig_hist->SetLineColor(kGreen+2);
    total_sig_hist->SetLineWidth(2);
    total_sig_hist->SetLineStyle(1);

    total_sb_hist->SetLineColor(kMagenta-3);
    total_sb_hist->SetLineWidth(2);
    total_sb_hist->SetLineStyle(1);

    double scale = cat_scale[cat];
    //    total_sig_hist->Scale(scale);
    TH1D * total_zsig_hist = (TH1D*)total_sb_hist->Clone("total_zsig_hist");
        
    TH1D * bkgdErr = (TH1D*)total_qcd_hist->Clone("bkgdErr");

    InitHist(bkgdErr,"","",kCyan,1001);

    for (int iB=1; iB<=nBins; ++iB) {
      total_zj_hist->SetBinError(iB,0.);
      total_bkg_hist->SetBinError(iB,0.);
      total_qcd_hist->SetBinError(iB,0.);
      total_sig_hist->SetBinError(iB,0.);
      total_sb_hist->SetBinError(iB,0.);
      double zsig = total_sig_hist->GetBinContent(iB)+total_zj_hist->GetBinContent(iB);
      total_zsig_hist->SetBinContent(iB,zsig);
      total_zsig_hist->SetBinError(iB,0.);
    }

    double frameYmin = 0;
    double frameYmax = scaleYupper*total_data_new_hist->GetMaximum();
    if (logY) 
      frameYmin = 1.0;
    
    total_data_new_hist->GetYaxis()->SetRangeUser(frameYmin,frameYmax);

    total_data_new_hist->GetXaxis()->SetLabelFont(42);
    total_data_new_hist->GetXaxis()->SetLabelOffset(0.04);
    total_data_new_hist->GetXaxis()->SetLabelSize(0.);
    total_data_new_hist->GetXaxis()->SetTickLength(0.04);
    total_data_new_hist->GetXaxis()->SetTitleOffset(1.2);
    total_data_new_hist->GetXaxis()->SetTitleSize(0.14);
    total_data_new_hist->GetXaxis()->SetTitle(xtitle);
    
    total_data_new_hist->GetYaxis()->SetLabelFont(42);
    total_data_new_hist->GetYaxis()->SetLabelSize(0.06);
    total_data_new_hist->GetYaxis()->SetTickLength(0.02);
    total_data_new_hist->GetYaxis()->SetLabelOffset(0.01);
    total_data_new_hist->GetYaxis()->SetTitleOffset(0.9);
    total_data_new_hist->GetYaxis()->SetTitleSize(0.08);
    total_data_new_hist->GetYaxis()->SetTitle("Events / 3 GeV");
    
    TCanvas * canv1 = MakeCanvas("canv1"+channel, "", 500, 600);
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
    upper->SetFrameLineWidth(3);
    upper->SetFrameBorderMode(0);
    upper->SetFrameBorderSize(10);
    upper->SetFrameFillStyle(0);
    upper->SetFrameLineStyle(0);
    upper->SetFrameLineWidth(3);
    upper->SetFrameBorderMode(0);
    upper->SetFrameBorderSize(10);


    total_data_new_hist->Draw("e1");
    bkgdErr->Draw("e2same");
    //    total_bkg_hist->Draw("hsame");
    total_qcd_hist->Draw("hsame");
    if (sbFit) {
      total_sb_hist->Draw("hsame");
      //      total_zj_hist->Draw("hsame");
      total_zsig_hist->Draw("hsame");
      total_sig_hist->Draw("hsame");
    }
    else {
      total_zj_hist->Draw("hsame");
    }
    total_data_new_hist->Draw("e1same");

    TLegend * leg  = new TLegend(0.2,0.15,0.6,0.45);
    SetLegendStyle(leg);
    leg->SetTextSize(0.045);
    leg->SetHeader(cat);
    leg->AddEntry(total_data_new_hist,"observed","lp");
    leg->AddEntry(total_qcd_hist,"QCD(+unc.)","l");
    if (sbFit) leg->AddEntry(total_sb_hist,"Z+Higgs","l");
    leg->AddEntry(total_sig_hist,"H#rightarrow b#bar{b}","l");
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

    TH1D * ratioH    = (TH1D*)total_data_new_hist->Clone("ratioH");
    TH1D * ratioErrH = (TH1D*)bkgdErr->Clone("ratioErrH");
    TH1D * ratioQCDH = (TH1D*)total_qcd_hist->Clone("ratioQCDH");

    ratioH->SetMarkerColor(1);
    ratioH->SetMarkerStyle(20);
    ratioH->SetMarkerSize(0.8);
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
    ratioH->GetYaxis()->SetTitle("Data-QCD");
    
    double ymax = -1.;
    double ymin =  1.;
    for (int iB=1; iB<=nBinsNew; ++iB) {
      double xData = total_data_new_hist->GetBinContent(iB);
      double xBkg  = total_qcd_new_hist->GetBinContent(iB);
      double eData = total_data_new_hist->GetBinError(iB);
      ratioH->SetBinContent(iB,xData-xBkg);
      ratioH->SetBinError(iB,eData);
      double x = ratioH->GetBinContent(iB)+ratioH->GetBinError(iB);
      if (x>ymax) ymax = x;
      x = ratioH->GetBinContent(iB)-ratioH->GetBinError(iB);
      if (x<ymin) ymin = x;
    }
    for (int iB=1; iB<=nBins; ++iB) {
      ratioErrH->SetBinContent(iB,0.);
      ratioQCDH->SetBinContent(iB,0.);
   }
 
    ratioH->GetYaxis()->SetRangeUser(1.1*ymin,1.1*ymax);    

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
    lower->SetFrameLineWidth(3);
    lower->SetFrameBorderMode(0);
    lower->SetFrameBorderSize(10);
    lower->SetFrameFillStyle(0);
    lower->SetFrameLineStyle(0);
    lower->SetFrameLineWidth(3);
    lower->SetFrameBorderMode(0);
    lower->SetFrameBorderSize(10);
    
    ratioH->Draw("e1");
    ratioErrH->Draw("e2same");
    total_sig_hist->Draw("hsame");
    total_zsig_hist->Draw("hsame");
    ratioH->Draw("e1same");
    TLine * line = new TLine(80,0,200,0);
    line->SetLineColor(4);
    line->SetLineWidth(2);
    line->Draw();

    lower->Modified();
    lower->RedrawAxis();
    canv1->cd();
    canv1->Modified();
    canv1->cd();
    canv1->SetSelected(canv1);
    canv1->Update();
    if (sbFit) canv1->Print("sb_"+cat+".png");
    else canv1->Print("bonly_"+cat+".png");
    delete canv1;
  }


  if (sbFit) {

    InitData(data_weighted_hist);

    data_weighted_hist->SetMarkerSize(0.8);

    InitSignal(qcd_weighted_hist);
    InitSignal(zj_weighted_hist);
    InitSignal(bkg_weighted_hist);
    InitSignal(sig_weighted_hist);
    InitSignal(sb_weighted_hist);

    qcd_weighted_hist->SetLineColor(4);
    qcd_weighted_hist->SetLineWidth(1);
    qcd_weighted_hist->SetLineStyle(1);
    
    bkg_weighted_hist->SetLineColor(2);
    bkg_weighted_hist->SetLineWidth(2);
    bkg_weighted_hist->SetLineStyle(1);
    
    zj_weighted_hist->SetLineColor(2);
    zj_weighted_hist->SetLineWidth(2);
    zj_weighted_hist->SetLineStyle(1);

    sig_weighted_hist->SetLineColor(kGreen+2);
    sig_weighted_hist->SetLineWidth(2);
    sig_weighted_hist->SetLineStyle(1);

    sb_weighted_hist->SetLineColor(kMagenta-3);
    sb_weighted_hist->SetLineWidth(2);
    sb_weighted_hist->SetLineStyle(1);

    TH1D * total_zsig_hist = (TH1D*)sb_weighted_hist->Clone("total_zsig_hist");
        
    TH1D * bkgdErr = (TH1D*)qcd_weighted_hist->Clone("bkgdErr");
    InitHist(bkgdErr,"","",kCyan,1001);
    bkgdErr->SetLineColor(4);
    bkgdErr->SetMarkerSize(0);

    for (int iB=1; iB<=nBinsNew; ++iB) {
      double error = TMath::Sqrt(qcd_weighted_hist->GetBinError(iB));
      double content = qcd_weighted_hist->GetBinContent(iB);
      //      std::cout << iB << " : " << content << " +/- " << error << std::endl;
      bkgdErr->SetBinError(iB,error);
      zj_weighted_hist->SetBinError(iB,0.);
      bkg_weighted_hist->SetBinError(iB,0.);
      qcd_weighted_hist->SetBinError(iB,0.);
      sig_weighted_hist->SetBinError(iB,0.);
      sb_weighted_hist->SetBinError(iB,0.);
      double zsig = sig_weighted_hist->GetBinContent(iB)+zj_weighted_hist->GetBinContent(iB);
      total_zsig_hist->SetBinContent(iB,zsig);
      total_zsig_hist->SetBinError(iB,0.);
    }

    double frameYmin = 0;
    double frameYmax = scaleYupper*data_weighted_hist->GetMaximum();
    if (logY) 
      frameYmin = 1.0;
    
    data_weighted_hist->GetYaxis()->SetRangeUser(frameYmin,frameYmax);

    data_weighted_hist->GetXaxis()->SetLabelFont(42);
    data_weighted_hist->GetXaxis()->SetLabelOffset(0.04);
    data_weighted_hist->GetXaxis()->SetLabelSize(0.);
    data_weighted_hist->GetXaxis()->SetTickLength(0.04);
    data_weighted_hist->GetXaxis()->SetTitleOffset(1.2);
    data_weighted_hist->GetXaxis()->SetTitleSize(0.14);
    data_weighted_hist->GetXaxis()->SetTitle(xtitle);
    
    data_weighted_hist->GetYaxis()->SetLabelFont(42);
    data_weighted_hist->GetYaxis()->SetLabelSize(0.06);
    data_weighted_hist->GetYaxis()->SetTickLength(0.02);
    data_weighted_hist->GetYaxis()->SetLabelOffset(0.01);
    data_weighted_hist->GetYaxis()->SetTitleOffset(0.9);
    data_weighted_hist->GetYaxis()->SetTitleSize(0.08);
    data_weighted_hist->GetYaxis()->SetTitle("S/(S+B) weighted");
    
    TCanvas * canv1 = MakeCanvas("canv1", "", 500, 600);
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
    upper->SetFrameLineWidth(3);
    upper->SetFrameBorderMode(0);
    upper->SetFrameBorderSize(10);
    upper->SetFrameFillStyle(0);
    upper->SetFrameLineStyle(0);
    upper->SetFrameLineWidth(3);
    upper->SetFrameBorderMode(0);
    upper->SetFrameBorderSize(10);
    
    TH1D * zj_scaled = (TH1D*)zj_weighted_hist->Clone("zj_scaled");
    TH1D * sig_scaled = (TH1D*)sig_weighted_hist->Clone("sig_scaled");
    //    zj_scaled->Scale(5.);
    //    sig_scaled->Scale(5.);

    //    data_weighted_hist->GetYaxis()->SetRangeUser(0,70);

    data_weighted_hist->Draw("e1");
    bkgdErr->Draw("e2lsame");
    //    bkg_weighted_hist->Draw("hsame");
    qcd_weighted_hist->Draw("hsame");
    zj_scaled->Draw("hsame");
    if (sbFit) {
      sig_scaled->Draw("hsame");
      sb_weighted_hist->Draw("hsame");
    }
    data_weighted_hist->Draw("e1same");

    TLegend * leg  = new TLegend(0.20,0.15,0.65,0.5);
    SetLegendStyle(leg);
    leg->SetTextSize(0.05);
    //    leg->SetHeader("S+B weighted");
    leg->AddEntry(data_weighted_hist,"observed","lp");
    leg->AddEntry(bkgdErr,"QCD(+unc.)","lf");
    if (sbFit) leg->AddEntry(sb_weighted_hist,"QCD+Z+Higgs","l");
    leg->AddEntry(zj_scaled,"Z#rightarrow b#bar{b}","l");
    leg->AddEntry(sig_scaled,"H#rightarrow b#bar{b}","l");
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

    TH1D * ratioH    = (TH1D*)data_weighted_hist->Clone("ratioH");
    TH1D * ratioErrH = (TH1D*)bkgdErr->Clone("ratioErrH");
    TH1D * ratioQCDH = (TH1D*)qcd_weighted_hist->Clone("ratioQCDH");

    ratioH->SetMarkerColor(1);
    ratioH->SetMarkerStyle(20);
    ratioH->SetMarkerSize(0.8);
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
    ratioH->GetYaxis()->SetTitle("Data-QCD");
    
    double ymax = -1.;
    double ymin =  1.;
    for (int iB=1; iB<=nBinsNew; ++iB) {
      double xData = data_weighted_hist->GetBinContent(iB);
      double xBkg  = qcd_weighted_hist->GetBinContent(iB);
      double eData = data_weighted_hist->GetBinError(iB);
      ratioH->SetBinContent(iB,xData-xBkg);
      ratioH->SetBinError(iB,eData);
      double x = ratioH->GetBinContent(iB) + ratioH->GetBinError(iB);
      if (x>ymax) ymax = x;
      x = ratioH->GetBinContent(iB) - ratioH->GetBinError(iB);
      if (x<ymin) ymin = x;
    }
    for (int iB=1; iB<=nBinsNew; ++iB) {
      ratioErrH->SetBinContent(iB,0.);
      ratioQCDH->SetBinContent(iB,0.);
   }

 
    ratioH->GetYaxis()->SetRangeUser(1.05*ymin,1.05*ymax);    

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
    lower->SetFrameLineWidth(3);
    lower->SetFrameBorderMode(0);
    lower->SetFrameBorderSize(10);
    lower->SetFrameFillStyle(0);
    lower->SetFrameLineStyle(0);
    lower->SetFrameLineWidth(3);
    lower->SetFrameBorderMode(0);
    lower->SetFrameBorderSize(10);
    
    total_zsig_hist->SetLineColor(kGreen+2);

    ratioH->Draw("e1");
    ratioErrH->Draw("e2same");
    zj_weighted_hist->Draw("hsame");
    total_zsig_hist->Draw("hsame");
    ratioH->Draw("e1same");
    TLine * line = new TLine(80,0,200,0);
    line->SetLineColor(4);
    line->Draw();

    lower->Modified();
    lower->RedrawAxis();
    canv1->cd();
    canv1->Modified();
    canv1->cd();
    canv1->SetSelected(canv1);
    canv1->Update();
    canv1->Print("sb_weighted.png");
    canv1->Print("sb_weighted.pdf");
  }


}
