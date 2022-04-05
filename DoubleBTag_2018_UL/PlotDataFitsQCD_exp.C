#include "HttStylesNew.cc"
#include "Common.h"
void PlotDataFitsQCD_exp(int iCAT=0, 
			 int iORDER=1,
			 bool blind=true,
			 bool BERN=false,
			 double ymin_ratio = 0.801,
			 double ymax_ratio = 1.199) {

  using namespace RooFit;
  TH1::SetDefaultSumw2(true);
  SetStyle();

  TString blinded("unblinded");
  if (blind) blinded = "blinded";

  TFile * file = new TFile(dirName+"/mbb_and_bdt_all_Nom.root");

  vector<TString> sample_names = {
    //    "WJets",
    "ZJets",
    //    "tt",
    //    "Sandgle_Top",
    "VBF_Hbb_Dipol",
    "ggF_Hbb_POWHEG"
  };

  TNtuple * tree = (TNtuple*)file->Get("Mass_and_BDT_DATA");

  TString Cuts = cuts[iCAT];
  TString CutsBlind = Cuts + "&&("+variable+"<"+Xmin_blind+"||"+variable+">"+Xmax_blind+")";

  TH1::SetDefaultSumw2(true);
  TH1D * hist;
  TH1D * histBlind;

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_";
  TString nameBlindHist = "mbb_blind_";

  hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  histBlind = new TH1D(nameBlindHist,"",NbinsBkg,xmin,xmax);

  tree->Draw(variable+">>"+nameHist,Cuts);
  tree->Draw(variable+">>"+nameBlindHist,CutsBlind);
  double yield_ini = hist->GetSumOfWeights();

  for (auto sample_name : sample_names) {
    TNtuple * tuple = (TNtuple*)file->Get("Mass_and_BDT_"+sample_name);
    TH1D * hist_sample = new TH1D(nameHist+sample_name,"",NbinsBkg,xmin,xmax);
    TH1D * histBlind_sample = new TH1D(nameBlindHist+sample_name,"",NbinsBkg,xmin,xmax);
    tuple->Draw(variable+">>"+nameHist+sample_name,"weight*("+Cuts+")");
    tuple->Draw(variable+">>"+nameBlindHist+sample_name,"weight*("+CutsBlind+")");
    if (sample_name=="ZJets") {
      hist_sample->Scale(znorm[iCAT]);
      histBlind_sample->Scale(znorm[iCAT]);
    }
    hist->Add(hist_sample,-1.);
    histBlind->Add(histBlind_sample,-1.);
  }

  delete dummy;

  double norm = hist->GetSumOfWeights();

  InitData(hist);
  InitData(histBlind);
  hist->SetMarkerSize(1.1);
  histBlind->SetMarkerSize(1.1);
  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  RooRealVar b0("b0_"+names[iCAT],"b0",0,300);
  RooRealVar b1("b1_"+names[iCAT],"b1",0,300);
  RooRealVar b2("b2_"+names[iCAT],"b2",0,300);
  RooRealVar b3("b3_"+names[iCAT],"b3",0,300);
  RooRealVar b4("b4_"+names[iCAT],"b4",0,300);
  RooRealVar b5("b5_"+names[iCAT],"b5",0,300);

  RooRealVar a0("a0_"+names[iCAT],"a0",-1.,1.);
  RooRealVar a1("a1_"+names[iCAT],"a1",-1.,1.);
  RooRealVar a2("a2_"+names[iCAT],"a2",-1.,1.);
  RooRealVar a3("a3_"+names[iCAT],"a3",-1.,1.);
  RooRealVar a4("a4_"+names[iCAT],"a4",-1.,1.);
  RooRealVar a5("a5_"+names[iCAT],"a5",-1,1);


  RooArgList argList_bern(b0,b1), argList_chv(a0,a1);
  RooArgList argListPolExp;

  if (iORDER>=1)
    argListPolExp.add(a1);
  if (iORDER>=2)
    argListPolExp.add(a2);

  if (iORDER>=3)
    {argList_bern.add(b2); argList_chv.add(a2); argListPolExp.add(a2);}
  if (iORDER>=4)
    {argList_bern.add(b3); argList_chv.add(a3);}
  if (iORDER>=5)
    {argList_bern.add(b4); argList_chv.add(a4);}
  if (iORDER>=6)
    {argList_bern.add(b5); argList_chv.add(a5);}

  RooRealVar tau_decay("tau_decay_"+names[iCAT],"tau_decay",-1.0e-02,-5.0e-03);
  tau_decay.setVal(-9.0e-03);
  argListPolExp.add(tau_decay);

  RooBernstein BRN("bern_"+names[iCAT],"Bernstein",mbb,argList_bern);
  RooGenericPdf EXP((string("qcd_")+namesCAT[iCAT]).c_str(),genericPolynomExp.at(iORDER).c_str(),RooArgSet(mbb,argListPolExp));
  RooDataHist data("data_"+names[iCAT],"data",mbb,hist);
  RooDataHist dataBlind("dataBlind","data",mbb,histBlind);
  TH1D * ratioHist = (TH1D*)hist->Clone("ratioHist");
  double chi2_ = 0.0;
  double ndof_ = 0.0;
  if(BERN)
  {
    std::cout << "Bernstein polynomial is going to fit" << std::endl;
    RooFitResult * fitRes = BRN.fitTo(data,Save());
    cout << endl;
    fitRes->Print();
  }
  else
  {  
    std::cout << "Chebyshev polynomial is going to fit" << std::endl;
    //    mbb.setRange("low",xmin,100);
    //    mbb.setRange("high",150,xmax);
    //    RooFitResult * fitRes = EXP.fitTo(data,SumW2Error(kTRUE),Save(),Range("low,high"),SplitRange(kFALSE));
    RooFitResult * fitRes = EXP.fitTo(data,SumW2Error(kTRUE),Save());
    cout << endl;
    fitRes->Print();
    RooAbsReal* intOfFunc = EXP.createIntegral(mbb);
    double integral = 0;
    double norm = 0;
    for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
      double xCenter = hist->GetBinCenter(iBin);
      mbb.setVal(xCenter);      
      integral += EXP.getVal();
      norm += hist->GetBinContent(iBin);
    }
    std::cout << "integral (model) = " << integral << std::endl;
    std::cout << "integral (histo) = " << norm << std::endl;
    double scale = norm/integral;

    for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
      double xCenter = hist->GetBinCenter(iBin);
      double xHist   = hist->GetBinContent(iBin);
      double xError  = hist->GetBinError(iBin);
      mbb.setVal(xCenter);
      double xModel  = scale*EXP.getVal();
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
	  ndof_ += 1.;
	}
      }
      else {
	chi2_ += term*term;
	ndof_ += 1.0;
      }
    }
  }
  RooPlot* xframe = mbb.frame();
  
  if (blind)
    data.plotOn(xframe,LineColor(0),MarkerColor(0),MarkerSize(1.1));
  else 
    data.plotOn(xframe,MarkerSize(1.1));

  if(BERN)
   {
    BRN.plotOn(xframe);
   }
  else
   {
    EXP.plotOn(xframe);
   }
  if (blind)
    dataBlind.plotOn(xframe,MarkerSize(1.1));

  double ymax = hist->GetMaximum();
  TH2D * frame = new TH2D("frame","",2,xmin,xmax,2,1,1.2*ymax);
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
  frame->GetYaxis()->SetTitle("Events");

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

  double chi2;
  if(BERN)
    {
      RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",BRN,data) ;
      chi2 = chi2Roo.getVal();
    }
  else
    {
      RooChi2Var chi2Roo =  RooChi2Var("chi2"+names[iCAT],"chi2",EXP,data) ;
      chi2 = chi2Roo.getVal();
    }
  double ndof  = NbinsBkg - iORDER - 1.;
  ndof_ = ndof_ - iORDER - 1.;
  double prob  = TMath::Prob(chi2,ndof);
  double prob_ = TMath::Prob(chi2_,ndof_);
  cout << endl;
  cout << "+++++++++++ " << names[iCAT] << " +++++++++++++" << endl;
  cout << " Cut : " << cuts[iCAT] << endl;
  cout << endl;
  cout << "Fitted range : Chi2/ndof = " << chi2_ << "/" << ndof_ << " = " << chi2_/ndof_ << "  prob = " << prob_ << std::endl;
  cout << "Full range   : Chi2/ndof = " << chi2 << "/" << ndof << " = " << chi2/ndof << "  prob = " << prob << endl;
  cout << endl;
  cout << "TOTAL DATA YIELD = " << yield_ini << endl;
  cout << "TOTAL QCD YIELD  = " << hist->GetSumOfWeights() << endl;
  cout << endl;
  char str1[20];
  sprintf(str1,"#chi^{2}/ndof = %4.2f",chi2_/ndof_);
  char str2[20];
  sprintf(str2,"P(#chi^{2}) = %4.2f",prob_);
  int pol_ord = 0;
  if(BERN)
   {
    pol_ord = iORDER-1;
   }
  else
   {
    pol_ord = iORDER;
   }
  char str3[20];
  if(BERN)
   {
    sprintf(str3,"Bernstein Pol. Ord: %d",pol_ord);
   }
  else  
   {
    sprintf(str3,"exp*pol%d",pol_ord);   
   }

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
  //  canv->Update();
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
  int cat_n = iCAT + 4;
  sprintf(str,"SingleB CAT %d",cat_n);
  TLatex * text = new TLatex(0.6,0.84,str);
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

  canv->Print("ExpPol_"+names[iCAT]+"_"+blinded+".png");
  

}
