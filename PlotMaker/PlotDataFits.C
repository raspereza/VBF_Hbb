#include "HttStylesNew.cc"
#include "Common.h"
void PlotDataFits(int iCAT=0, 
		  int iORDER=3,
		  bool blind=true,
                  bool BERN=false) {

  using namespace RooFit;

  SetStyle();

  TString blinded("unblinded");
  if (blind) blinded = "blinded";

  TFile * file = new TFile("mbb_and_bdt_all_Nom.root");
  TNtuple * tree = (TNtuple*)file->Get("Mass_and_BDT_DATA");
  TNtuple * wjmc = (TNtuple*)file->Get("Mass_and_BDT_WJets");
  TNtuple * zjmc = (TNtuple*)file->Get("Mass_and_BDT_ZJets");
  TNtuple * ttmc  = (TNtuple*)file->Get("Mass_and_BDT_tt");
  TNtuple * stmc  = (TNtuple*)file->Get("Mass_and_BDT_Sandgle_Top");

  TString cutsBlind;
  cutsBlind = cuts[iCAT] + "&&(mbb_reg<100||mbb_reg>150)";

  TH1::SetDefaultSumw2(true);
  TH1D * hist;
  TH1D * hist_wj;
  TH1D * hist_zj;
  TH1D * hist_tt;
  TH1D * hist_st;
  TH1D * histBlind;
  TH1D * histBlind_tt;
  TH1D * histBlind_st;
  TH1D * histBlind_wj;
  TH1D * histBlind_zj;


  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_";
  TString nameHist_wj = "mbb_wj_";
  TString nameHist_zj = "mbb_zj_";
  TString nameHist_tt = "mbb_tt_";
  TString nameHist_st = "mbb_st_";

  TString nameBlindHist = "mbb_blind";
  TString nameBlindHist_tt = "mbb_blind_tt";
  TString nameBlindHist_st = "mbb_blind_st";
  TString nameBlindHist_wj = "mbb_blind_wj";
  TString nameBlindHist_zj = "mbb_blind_zj";

  hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  hist_wj = new TH1D(nameHist_wj,"",NbinsBkg,xmin,xmax);
  hist_zj = new TH1D(nameHist_zj,"",NbinsBkg,xmin,xmax);
  hist_tt = new TH1D(nameHist_tt,"",NbinsBkg,xmin,xmax);
  hist_st = new TH1D(nameHist_st,"",NbinsBkg,xmin,xmax);


  histBlind = new TH1D(nameBlindHist,"",NbinsBkg,xmin,xmax);
  histBlind_wj = new TH1D(nameBlindHist_wj,"",NbinsBkg,xmin,xmax);
  histBlind_zj = new TH1D(nameBlindHist_zj,"",NbinsBkg,xmin,xmax);
  histBlind_tt = new TH1D(nameBlindHist_tt,"",NbinsBkg,xmin,xmax);
  histBlind_st = new TH1D(nameBlindHist_st,"",NbinsBkg,xmin,xmax);

  tree->Draw("mbb_reg>>"+nameHist,cuts[iCAT]);
  wjmc->Draw("mbb_reg>>"+nameHist_wj,"weight*("+cuts[iCAT]+")");
  zjmc->Draw("mbb_reg>>"+nameHist_zj,"weight*("+cuts[iCAT]+")");
  ttmc->Draw("mbb_reg>>"+nameHist_tt,"weight*("+cuts[iCAT]+")");
  stmc->Draw("mbb_reg>>"+nameHist_st,"weight*("+cuts[iCAT]+")");

  tree->Draw("mbb_reg>>"+nameBlindHist,cutsBlind);
  wjmc->Draw("mbb_reg>>"+nameBlindHist_wj,"weight*("+cutsBlind+")");
  zjmc->Draw("mbb_reg>>"+nameBlindHist_zj,"weight*("+cutsBlind+")");
  ttmc->Draw("mbb_reg>>"+nameBlindHist_tt,"weight*("+cutsBlind+")");
  stmc->Draw("mbb_reg>>"+nameBlindHist_st,"weight*("+cutsBlind+")");

  delete dummy;
  hist->Add(hist_wj,-1);
  hist->Add(hist_zj,-1);
  hist->Add(hist_tt,-1);
  hist->Add(hist_st,-1);

  histBlind->Add(histBlind_wj,-1);
  histBlind->Add(histBlind_zj,-1);
  histBlind->Add(histBlind_tt,-1);
  histBlind->Add(histBlind_st,-1);

  InitData(hist);
  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);
  RooRealVar b0("b0_"+names[iCAT],"b0",0,300);
  RooRealVar b1("b1_"+names[iCAT],"b1",0,300);
  RooRealVar b2("b2_"+names[iCAT],"b2",0,300);
  RooRealVar b3("b3_"+names[iCAT],"b3",0,300);
  RooRealVar b4("b4_"+names[iCAT],"b4",0,300);
  RooRealVar b5("b5_"+names[iCAT],"b5",0,300);
  RooRealVar a0("a0_"+names[iCAT],"a0",-1,1);
  RooRealVar a1("a1_"+names[iCAT],"a1",-1,1);
  RooRealVar a2("a2_"+names[iCAT],"a2",-1,1);
  RooRealVar a3("a3_"+names[iCAT],"a3",-1,1);
  RooRealVar a4("a4_"+names[iCAT],"a4",-1,1);
  RooRealVar a5("a5_"+names[iCAT],"a5",-1,1);


  RooArgList argList_bern(b0,b1), argList_chv(a0,a1);
  if (iORDER>=3)
    {argList_bern.add(b2); argList_chv.add(a2);}
  if (iORDER>=4)
    {argList_bern.add(b3); argList_chv.add(a3);}
  if (iORDER>=5)
    {argList_bern.add(b4); argList_chv.add(a4);}
  if (iORDER>=6)
    {argList_bern.add(b5); argList_chv.add(a5);}

  RooBernstein BRN("qcd_"+names[iCAT],"Bernstein",mbb,argList_bern);
  RooChebychev CHV("qcd_"+names[iCAT],"chebysev",mbb,argList_chv);
  RooDataHist data("data_"+names[iCAT],"data",mbb,hist);
  RooDataHist dataBlind("dataBlind","data",mbb,histBlind);
  if(BERN)
  {
    std::cout << "Bernstein polynomial is going to fit" << std::endl;
    RooFitResult * fitRes = BRN.fitTo(data,Save());
    cout << endl;
    fitRes->Print();
  }
  else
  {  
    std::cout << "Chebysev polynomial is going to fit" << std::endl;
    RooFitResult * fitRes = CHV.fitTo(data,SumW2Error(kFALSE),Save());
    cout << endl;
    fitRes->Print();
  }
  RooPlot* xframe = mbb.frame();
  if (blind)
    data.plotOn(xframe,LineColor(0),MarkerColor(0));
  else 
    data.plotOn(xframe);
  if(BERN)
   {
    BRN.plotOn(xframe);
   }
  else
   {
    CHV.plotOn(xframe);
   }
  if (blind)
    dataBlind.plotOn(xframe);
  xframe->SetTitle("");
  xframe->SetLineColor(kGreen-1);
  xframe->SetLineWidth(2);
  xframe->GetYaxis()->SetTitle("Events / 1 GeV");
  xframe->GetXaxis()->SetTitle("m_{bb} [GeV]");
  xframe->GetYaxis()->SetTitleOffset(1.6);
  double chi2;
 if(BERN)
  {
     RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",BRN,data) ;
     chi2 = chi2Roo.getVal();
  }
 else
  {
     RooChi2Var chi2Roo =  RooChi2Var("chi2"+names[iCAT],"chi2",CHV,data) ;
     chi2 = chi2Roo.getVal();
  }
  double ndof = NbinsBkg - iORDER;
  double prob = TMath::Prob(chi2,ndof);
  cout << "Chi2/ndof = " << chi2 << "/" << ndof << " = " << chi2/ndof << "  prob = " << prob << endl;
  cout << "HISTOGRAM ENTRIES = " << hist->GetSumOfWeights() << endl;
  char str1[20];
  sprintf(str1,"#chi^{2}/ndof = %4.2f",chi2/ndof);
  char str2[20];
  sprintf(str2,"P(#chi^{2}) = %4.2f",prob);
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
    sprintf(str3,"Chebysev Pol. Ord: %d",pol_ord);   
   }
  TCanvas * canv = new TCanvas("canv","",700,700);
  canv->Range(0,0,1,1);
  canv->SetFillColor(0);
  canv->SetBorderMode(0);
  canv->SetBorderSize(2);
  canv->SetFrameBorderMode(0);
  canv->SetFillColor(0);
  canv->SetBorderMode(0);
  canv->SetBorderSize(3);
  canv->SetGridx();
  canv->SetGridy();
  canv->SetTickx(1);
  canv->SetTicky(1);
  canv->SetLeftMargin(0.160401);
  canv->SetRightMargin(0.04135338);
  canv->SetBottomMargin(0.15);
  canv->SetFrameBorderMode(0);
  canv->SetFrameLineWidth(2);
  canv->SetFrameBorderMode(0);
  canv->Draw();
  xframe->SetTitle("");
  xframe->GetXaxis()->SetTitle("m_{bb} [GeV] ");
  xframe->GetXaxis()->SetNdivisions(506);
  xframe->GetXaxis()->SetLabelFont(132);
  xframe->GetXaxis()->SetLabelSize(0.050);
  xframe->GetXaxis()->SetTitleSize(0.05);
  xframe->GetXaxis()->SetTickLength(0.075);
  xframe->GetXaxis()->SetTitleFont(22);
  xframe->GetXaxis()->SetTitleOffset(1.00);
  xframe->GetYaxis()->SetTitle("Events / 1 GeV");
  xframe->GetYaxis()->SetNdivisions(506);
  xframe->GetYaxis()->SetLabelFont(132);
  xframe->GetYaxis()->SetLabelSize(0.050);
  xframe->GetYaxis()->SetTitleSize(0.05);
  xframe->GetYaxis()->SetTitleFont(22);
  xframe->GetYaxis()->SetTitleOffset(1.30);
  xframe->Draw();
  canv->Update();
  TLegend * leg = new TLegend(0.7,0.75,0.85,0.8);
  SetLegendStyle(leg);
  leg->AddEntry(hist,"Data","lp");
  leg->Draw();
  TLatex * text3 = new TLatex(0.25,0.45,str3);
  TLatex * text1 = new TLatex(0.25,0.35,str1);
  TLatex * text2 = new TLatex(0.25,0.25,str2);
  text1->SetNDC();
  text2->SetNDC();
  text3->SetNDC();
  text1->SetTextSize(0.04);
  text1->SetTextFont(22);
  text2->SetTextSize(0.04);
  text2->SetTextFont(22);
  text3->SetTextSize(0.04);
  text3->SetTextFont(22);
  text1->Draw();
  text2->Draw();
  text3->Draw();
  char str[20];
  int cat_n = iCAT + 4;
  sprintf(str,"SingleB CAT %d",cat_n);
  TLatex * text = new TLatex(0.70,0.84,str);
  text->SetNDC();
  text->SetTextFont(22);
  text->SetTextSize(0.04);
  text->SetTextColor(kBlack);
  text->Draw();

  TLatex *t2a = new TLatex(0.58,0.94,"#bf{CMS} #it{Preliminary}          36.3 fb^{-1} (13 TeV)  ");
  t2a->SetNDC();
  t2a->SetTextFont(42);
  t2a->SetTextSize(0.05);
  t2a->SetTextAlign(20);
  t2a->Draw("same");

  canv->Update();
  if(BERN)
    canv->Print("QCD_modelling_"+names[iCAT]+"_"+blinded+"_BERN_POL_DB.png");
  else 
    {
	    canv->Print("Plots/png/2016_UL_SB_DATA_modelling_"+names[iCAT]+"_"+blinded+"_CHEV_POL.png");
            canv->Print("Plots/pdf/2016_UL_SB_DATA_modelling_"+names[iCAT]+"_"+blinded+"_CHEV_POL.pdf");
    }
}
