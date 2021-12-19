#include "HttStylesNew.cc"
#include "Common.h"
void PlotDataFitsTF(int iCAT=4, 
		    int iORDER=3,
		    TString variable = "mbb_reg",
		    bool blind=true,
		    double ymin_ratio = 0.8,
		    double ymax_ratio = 1.21) {

  using namespace RooFit;

  SetStyle();

  TString blinded("unblinded");
  if (blind) blinded = "blinded";

  TFile * file = new TFile(dirName+"/mbb_and_bdt_all_Nom.root");

  TNtuple * tree = (TNtuple*)file->Get("Mass_and_BDT_DATA");

  TNtuple * wjmc = (TNtuple*)file->Get("Mass_and_BDT_WJets");
  TNtuple * zjmc = (TNtuple*)file->Get("Mass_and_BDT_ZJets");
  TNtuple * ttmc  = (TNtuple*)file->Get("Mass_and_BDT_tt");
  TNtuple * stmc  = (TNtuple*)file->Get("Mass_and_BDT_Sandgle_Top");
  TNtuple * vbfmc  = (TNtuple*)file->Get("Mass_and_BDT_VBF_Hbb_Dipol");
  TNtuple * ggfmc  = (TNtuple*)file->Get("Mass_and_BDT_ggF_Hbb");

  TString Cuts0 = cuts[0];
  TString Cuts = cuts[iCAT];

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  TH1D * hist;
  TH1D * hist_wj;
  TH1D * hist_zj;
  TH1D * hist_tt;
  TH1D * hist_st;
  TH1D * hist_ggf;
  TH1D * hist_vbf;

  TH1D * hist0;
  TH1D * hist0_wj;
  TH1D * hist0_zj;
  TH1D * hist0_tt;
  TH1D * hist0_st;
  TH1D * hist0_ggf;
  TH1D * hist0_vbf;

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_";
  TString nameHist_wj = "mbb_wj_";
  TString nameHist_zj = "mbb_zj_";
  TString nameHist_tt = "mbb_tt_";
  TString nameHist_st = "mbb_st_";
  TString nameHist_vbf = "mbb_vbf_";
  TString nameHist_ggf = "mbb_ggf_";

  TString nameHist0 = "mbb0_";
  TString nameHist0_wj = "mbb0_wj_";
  TString nameHist0_zj = "mbb0_zj_";
  TString nameHist0_tt = "mbb0_tt_";
  TString nameHist0_st = "mbb0_st_";
  TString nameHist0_vbf = "mbb0_vbf_";
  TString nameHist0_ggf = "mbb0_ggf_";

  hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  hist_wj = new TH1D(nameHist_wj,"",NbinsBkg,xmin,xmax);
  hist_zj = new TH1D(nameHist_zj,"",NbinsBkg,xmin,xmax);
  hist_tt = new TH1D(nameHist_tt,"",NbinsBkg,xmin,xmax);
  hist_st = new TH1D(nameHist_st,"",NbinsBkg,xmin,xmax);
  hist_vbf = new TH1D(nameHist_vbf,"",NbinsBkg,xmin,xmax);
  hist_ggf = new TH1D(nameHist_ggf,"",NbinsBkg,xmin,xmax);

  hist0 = new TH1D(nameHist0,"",NbinsBkg,xmin,xmax);
  hist0_wj = new TH1D(nameHist0_wj,"",NbinsBkg,xmin,xmax);
  hist0_zj = new TH1D(nameHist0_zj,"",NbinsBkg,xmin,xmax);
  hist0_tt = new TH1D(nameHist0_tt,"",NbinsBkg,xmin,xmax);
  hist0_st = new TH1D(nameHist0_st,"",NbinsBkg,xmin,xmax);
  hist0_vbf = new TH1D(nameHist0_vbf,"",NbinsBkg,xmin,xmax);
  hist0_ggf = new TH1D(nameHist0_ggf,"",NbinsBkg,xmin,xmax);

  tree->Draw(variable+">>"+nameHist,Cuts);
  wjmc->Draw(variable+">>"+nameHist_wj,"weight*("+Cuts+")");
  zjmc->Draw(variable+">>"+nameHist_zj,"weight*("+Cuts+")");
  ttmc->Draw(variable+">>"+nameHist_tt,"weight*("+Cuts+")");
  stmc->Draw(variable+">>"+nameHist_st,"weight*("+Cuts+")");
  vbfmc->Draw(variable+">>"+nameHist_vbf,"weight*("+Cuts+")");
  ggfmc->Draw(variable+">>"+nameHist_ggf,"weight*("+Cuts+")");

  tree->Draw(variable+">>"+nameHist0,Cuts0);
  wjmc->Draw(variable+">>"+nameHist0_wj,"weight*("+Cuts0+")");
  zjmc->Draw(variable+">>"+nameHist0_zj,"weight*("+Cuts0+")");
  ttmc->Draw(variable+">>"+nameHist0_tt,"weight*("+Cuts0+")");
  stmc->Draw(variable+">>"+nameHist0_st,"weight*("+Cuts0+")");
  vbfmc->Draw(variable+">>"+nameHist0_vbf,"weight*("+Cuts0+")");
  ggfmc->Draw(variable+">>"+nameHist0_ggf,"weight*("+Cuts0+")");

  double yield_ini = hist->GetSumOfWeights();

  delete dummy;

  hist->Add(hist_wj,-1);
  hist->Add(hist_zj,-1);
  hist->Add(hist_tt,-1);
  hist->Add(hist_st,-1);
  hist->Add(hist_vbf,-1);
  hist->Add(hist_ggf,-1);

  hist0->Add(hist0_wj,-1);
  hist0->Add(hist0_zj,-1);
  hist0->Add(hist0_tt,-1);
  hist0->Add(hist0_st,-1);
  hist0->Add(hist0_vbf,-1);
  hist0->Add(hist0_ggf,-1);

  TH1D * ratio = DivideHist(hist,hist0,"ratio");
  TH1D * ratioBlind = (TH1D*)ratio->Clone("ratioBlind");
  TH1D * ratioHist = (TH1D*)ratio->Clone("ratioHist");
  int nBins = ratioBlind->GetNbinsX();
  for (int iB=1; iB<=nBins; ++iB) {
    double x = ratioBlind->GetXaxis()->GetBinCenter(iB);
    if (x>xmin_blind&&x<xmax_blind) {
      ratioBlind->SetBinContent(iB,-1.);
      ratioBlind->SetBinError(iB,0.);
    }
  }

  InitData(ratio);
  InitData(ratioBlind);
  InitData(ratioHist);
  ratio->SetMarkerSize(1.1);
  ratioBlind->SetMarkerSize(1.1);
  ratioHist->SetMarkerSize(1.1);

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  RooRealVar a1("a1_"+names[iCAT],"a1",-5,5);
  RooRealVar a2("a2_"+names[iCAT],"a2",-5,5);
  RooRealVar a3("a3_"+names[iCAT],"a3",-5,5);
  RooRealVar a4("a4_"+names[iCAT],"a4",-5,5);
  RooRealVar a5("a5_"+names[iCAT],"a5",-5,5);

  RooArgList argListTF(a1);

  if (iORDER>=2)
    argListTF.add(a2);
  if (iORDER>=3)
    argListTF.add(a3);
  if (iORDER>=4)
    argListTF.add(a4);
  if (iORDER>=5)
    argListTF.add(a5);

  RooGenericPdf tf(polynomNames.at(iORDER).c_str(),genericPolynoms.at(iORDER).c_str(),RooArgSet(mbb,argListTF));

  RooDataHist data("data_"+names[iCAT],"data",mbb,ratio);
  RooDataHist dataBlind("dataBlind","data",mbb,ratioBlind);
  
  std::cout << "Fitting TF -------> " << std::endl;
  //  mbb.setRange("low",xmin,100);
  //  mbb.setRange("high",150,xmax);
  RooFitResult * fitRes = tf.fitTo(data,SumW2Error(kTRUE),Save());
  fitRes->Print();
  RooAbsReal* intOfFunc = tf.createIntegral(mbb);
  double integral = 0;
  double norm = 0;
  for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
    double xCenter = hist->GetBinCenter(iBin);
    mbb.setVal(xCenter);      
    integral += tf.getVal();
    norm += ratio->GetBinContent(iBin);
  }
  std::cout << endl;
  std::cout << "integral (model) = " << integral << std::endl;
  std::cout << "integral (pol)   = " << norm << std::endl;
  double scale = norm/integral;
  cout << endl;

  double chi2_ = 0;
  double ndof_ = 0;
  for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
    double xCenter = ratio->GetBinCenter(iBin);
    double xHist   = ratio->GetBinContent(iBin);
    double xError  = ratio->GetBinError(iBin);
    mbb.setVal(xCenter);
    double xModel  = scale*tf.getVal();
    double term = (xHist-xModel)/xError;
    double rat = xHist/xModel;
    double ratError = xError/xModel;
    ratioHist->SetBinContent(iBin,rat);
    ratioHist->SetBinError(iBin,ratError);
    if (blind) {
      if (xCenter>xmin_blind&&xCenter<xmax_blind) {
	ratioHist->SetBinContent(iBin,-1.);
	ratioHist->SetBinError(iBin,0.);
      }
      else {
	chi2_ += term*term;
	ndof_ += 1.0;
      }    
    }
    else {
      chi2_ += term*term;
      ndof_ += 1.0;
    }
  }

  RooPlot* xframe = mbb.frame();
  
  if (blind)
    data.plotOn(xframe,LineColor(0),MarkerColor(0),MarkerSize(1.1));
  else 
    data.plotOn(xframe,MarkerSize(1.1));
  tf.plotOn(xframe);
  if (blind)
    dataBlind.plotOn(xframe,MarkerSize(1.1));

  TH2D * frame = new TH2D("frame","",2,xmin,xmax,2,0.01,2);
  TH2D * frameRatio = new TH2D("frameRatio","",2,xmin,xmax,2,ymin_ratio,ymax_ratio);

  frame->SetTitle("");
  frame->GetXaxis()->SetTitle("");
  frame->GetXaxis()->SetNdivisions(506);
  frame->GetXaxis()->SetLabelSize(0.0);
  frame->GetXaxis()->SetTitleSize(0.0);
  frame->GetXaxis()->SetTickLength(0.04);

  frame->GetYaxis()->SetNdivisions(506);
  frame->GetYaxis()->SetLabelFont(42);
  frame->GetXaxis()->SetTickLength(0.04);
  frame->GetYaxis()->SetLabelSize(0.045);
  //  frame->GetYaxis()->SetTitleSize(0.05);
  frame->GetYaxis()->SetTitleOffset(1.6);
  frame->GetYaxis()->SetTitle("Transfer Function");

  frameRatio->SetMarkerColor(1);
  frameRatio->SetMarkerStyle(20);
  frameRatio->SetMarkerSize(1.2);
  frameRatio->SetLineColor(1);
  frameRatio->GetYaxis()->SetRangeUser(0.901,1.099);
  frameRatio->GetYaxis()->SetNdivisions(505);
  frameRatio->GetYaxis()->SetTitleOffset(1.8);
  frameRatio->GetXaxis()->SetLabelFont(42);
  //  frameRatio->GetXaxis()->SetTitleFont(22);
  frameRatio->GetXaxis()->SetLabelOffset(0.04);
  frameRatio->GetXaxis()->SetLabelSize(0.14);
  frameRatio->GetXaxis()->SetTitleOffset(3.4);
  frameRatio->GetXaxis()->SetTickLength(0.07);
  frameRatio->GetYaxis()->SetTitle("obs/fit");
  frameRatio->GetYaxis()->SetLabelFont(42);
  frameRatio->GetYaxis()->SetLabelOffset(0.015);
  frameRatio->GetYaxis()->SetLabelSize(0.11);
  frameRatio->GetYaxis()->SetTickLength(0.04);
  frameRatio->GetXaxis()->SetTitle("m_{bb} (GeV)");


  RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",tf,data) ;
  double chi2 = chi2Roo.getVal();
  double ndof = NbinsBkg - iORDER;
  double prob = TMath::Prob(chi2,ndof);

  ndof_ = ndof_ - iORDER;
  double prob_ = TMath::Prob(chi2_,ndof_);
  cout << endl;
  cout << "+++++++++++ " << names[iCAT] << " +++++++++++++" << endl;
  cout << "Fitted range : Chi2/ndof = " << chi2_ << "/" << ndof_<< "   prob = " << prob_ << endl;
  cout << "Full range   : Chi2/ndof = " << chi2 << "/" << ndof << " = " << chi2/ndof << "  prob = " << prob << endl;
  cout << endl;
  cout << "TOTAL DATA YIELD = " << yield_ini << endl; 
  cout << "TOTAL QCD YIELD  = " << hist->GetSumOfWeights() << endl;
  cout << endl;
  char str1[20];
  sprintf(str1,"#chi^{2}/ndof = %4.2f",chi2/ndof);
  char str2[20];
  sprintf(str2,"P(#chi^{2}) = %4.2f",prob);
  int pol_ord = iORDER;
  char str3[20];
  sprintf(str3,"Pol. Ord: %d",pol_ord);

  //
  // Plotting --->
  //
  TCanvas * canv = new TCanvas("canv","",550,650);
  TPad *upper = new TPad("upper", "pad",0,0.31,1,1);
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

  frame->Draw();
  xframe->Draw("same");
  TLatex * text3 = new TLatex(0.22,0.3,str3);
  TLatex * text1 = new TLatex(0.22,0.2,str1);
  TLatex * text2 = new TLatex(0.22,0.1,str2);
  text1->SetNDC();
  text2->SetNDC();
  text3->SetNDC();
  text1->SetTextSize(0.05);
  text1->SetTextFont(22);
  text2->SetTextSize(0.05);
  text2->SetTextFont(22);
  text3->SetTextSize(0.05);
  text3->SetTextFont(22);
  text1->Draw();
  text2->Draw();
  text3->Draw();
  char str[20];
  int cat_n = iCAT;
  sprintf(str,"SingleB  CAT%d",cat_n+4);
  TLatex * text = new TLatex(0.5,0.8,str);
  text->SetNDC();
  text->SetTextFont(22);
  text->SetTextSize(0.06);
  text->SetTextColor(kBlack);
  text->Draw();
  upper->Draw("SAME");
  upper->RedrawAxis();
  upper->Modified();
  upper->Update();
  canv->cd();
  TPad * lower = new TPad("lower", "pad",0,0,1,0.31);
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

  frameRatio->Draw();
  TLine * line = new TLine(xmin,1.,xmax,1.);
  line->SetLineColor(kBlue);
  line->Draw();
  ratioHist->Draw("e1same");

  lower->Modified();
  lower->RedrawAxis();
  canv->cd();
  canv->Modified();
  canv->cd();
  canv->SetSelected(canv);
  canv->Update();
  //  canv->Print("TF_"+names[iCAT]+"_"+blinded+"POL.png");

}
