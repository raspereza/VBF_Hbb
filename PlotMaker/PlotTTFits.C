#include "HttStylesNew.cc"
#include "Common_MC.h"

void PlotTTFits(int iCAT=0) {

  using namespace RooFit;

  TH1::SetDefaultSumw2(true);
  
  SetStyle();
  TFile * file = new TFile("mbb_and_bdt_all_Nom.root");
  TNtuple * treeTT = (TNtuple*)file->Get("Mass_and_BDT_tt");
  TNtuple * treeST = (TNtuple*)file->Get("Mass_and_BDT_Sandgle_Top");
  

  TH1D * histTT;
  TH1D * histTT1;
  TH1D * histST;
  

  Float_t yieldTT;
  Float_t yieldST;

  TString nameSTHist = "mbb_st";  
  TString nameTTHist = "mbb_tt";
  TString nameTTHist1 = "mbb_tt1";//Definition of one extra histogram is just for stack plotting
  histST = new TH1D(nameSTHist,"",NbinsSig,xmin,xmax);
  histTT = new TH1D(nameTTHist,"",NbinsSig,xmin,xmax);
  histTT1 = new TH1D(nameTTHist1,"",NbinsSig,xmin,xmax);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  treeST->Draw("mbb_reg>>"+nameSTHist,"weight*("+cuts[iCAT]+")");
  treeTT->Draw("mbb_reg>>"+nameTTHist,"weight*("+cuts[iCAT]+")");
  treeTT->Draw("mbb_reg>>"+nameTTHist1,"weight*("+cuts[iCAT]+")");
  histTT->Add(histST);

  delete dummy;
  InitData(histTT);
  InitHist(histST,"","",kBlue-9,1001);
  InitHist(histTT1,"","",kGreen-9,1001);

  for (int iB=1; iB<120; ++iB)
{
    histST->SetBinError(iB,0);
    histTT1->SetBinError(iB,0);
}

  THStack *hs = new THStack("hs","");//stack histogram -- just to stack plot of two signals ggF & VBF 
  hs->Add(histST);
  hs->Add(histTT1);
  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);
  RooRealVar mean("mean_sig_"+names[iCAT],"Mean",125,80,200);
  RooRealVar meanScale("CMS_mbb_scale","Mean",125,80,200);
  RooRealVar sigma("sigma_sig_"+names[iCAT],"Width",10,0,30);
  RooRealVar alpha("alpha_sig_"+names[iCAT],"Alpha",0.5,0,2.);
  RooRealVar n("n_sig_"+names[iCAT],"n",10,0,20);
  RooRealVar b0("b0_sig_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_sig_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_sig_"+names[iCAT],"b2",0,1);
  RooRealVar fsig("fsig_"+names[iCAT],"fsig",0,1);
  RooRealVar mean_scale("CMS_vbfbb_scale_mbb_seldouble_13TeV_2018","Mbb_scale",1.0,0.5,1.5);
  RooRealVar sigma_res("CMS_vbfbb_res_mbb_seldouble_13TeV_2018","Mbb_scale",1.0,0.5,1.5);
  RooFormulaVar mean_shifted("mean_shifted_sig","@0*@1",RooArgList(mean,mean_scale));
  RooFormulaVar sigma_shifted("sigma_res_sig","@0*@1",RooArgList(sigma,sigma_res));
  RooBernstein BRN("brn_sig_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cbx("cb_sig_"+names[iCAT],"CBshape",mbb,mean,sigma,alpha,n);
  RooAddPdf signalx("signalx","signal",RooArgList(cbx,BRN),fsig);
  RooDataHist data("dataTT","dataTT",mbb,histTT);
  RooFitResult * res =  signalx.fitTo(data,Save());
  RooPlot * frame = mbb.frame();
  data.plotOn(frame);
  signalx.plotOn(frame);
  signalx.plotOn(frame,Components(BRN),LineStyle(kDashed));
  TObject *pdfLeg = frame->getObject(int(frame->numItems()-2));
  TCanvas *canv = new TCanvas("canv","canv",700,700);
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
  frame->SetTitle("");
  frame->GetXaxis()->SetTitle("m_{bb} [GeV] ");
  frame->GetXaxis()->SetNdivisions(506);
  frame->GetXaxis()->SetLabelFont(132);
  frame->GetXaxis()->SetLabelSize(0.050);
  frame->GetXaxis()->SetTitleSize(0.05);
  frame->GetXaxis()->SetTickLength(0.075);
  frame->GetXaxis()->SetTitleFont(22);
  frame->GetXaxis()->SetTitleOffset(1.00);
  frame->GetYaxis()->SetTitle("Events / 5 GeV");
  frame->GetYaxis()->SetNdivisions(506);
  frame->GetYaxis()->SetLabelFont(132);
  frame->GetYaxis()->SetLabelSize(0.050);
  frame->GetYaxis()->SetTitleSize(0.05);
  frame->GetYaxis()->SetTitleFont(22);
  frame->GetYaxis()->SetTitleOffset(1.30);
  frame->GetYaxis()->SetRangeUser(0.,1.3*histTT->GetMaximum());
  frame->Draw();
  hs->Draw("same");
  frame->Draw("same");
  TLegend * leg = new TLegend(0.632092,0.75896,0.9038968,0.907281);
  SetLegendStyle(leg);
  leg->SetTextSize(0.035);
  leg->AddEntry(histST,"Single Top","f");
  leg->AddEntry(histTT1,"t#bar{t}","f");
  leg->AddEntry(pdfLeg,"Parametric Fit", "L");
  leg->Draw();

  TPaveText *fmText = new TPaveText(0.6432665,0.7300594,0.8438395,0.7505795,"brNDC");
  fmText->SetFillColor(0);
  fmText->SetLineColor(kWhite);
  fmText->SetTextSize(0.037);
  fmText->AddText(Form("#mu = %1.2f GeV",mean.getVal()));

  TPaveText *fstdText = new TPaveText(0.6203438,0.6850817,0.8209169,0.7256018,"brNDC");
  fstdText->SetFillColor(0);
  fstdText->SetLineColor(kWhite);
  fstdText->SetTextSize(0.037);
  fstdText->AddText(Form("#sigma_{eff} = %1.2f GeV",sigma.getVal()));

  TPaveText *fwhmText = new TPaveText(0.6045845,0.6515899,0.7851003,0.6806241,"brNDC");
  fwhmText->SetFillColor(0);
  fwhmText->SetLineColor(kWhite);
  fwhmText->SetTextSize(0.037);
  fwhmText->AddText(Form("FWHM = %1.2f GeV",2.355*sigma.getVal()));

  fmText->Draw("same");
  fstdText->Draw("same");
  fwhmText->Draw("same");

  TLatex *   tex_cms = new TLatex(0.56,0.94," #bf{CMS} #it{Simulation Preliminary}             54.4 fb^{-1}(2018)");
  tex_cms->SetNDC();
  tex_cms->SetTextAlign(20);
  tex_cms->SetTextFont(42);
  tex_cms->SetTextSize(0.04);
  tex_cms->SetLineWidth(2);
  tex_cms->Draw();
  char str[20];
  int cat_n = iCAT + 0;
  sprintf(str,"SingleB ");
  TLatex * text = new TLatex(0.18,0.84,str);
  text->SetNDC();
  text->SetTextFont(22);
  text->SetTextSize(0.04);
  text->SetTextColor(kBlack);
  text->Draw();
/*
  for(int i =1 ; i <= NbinsSig ; i++ )
  {
    double exp = histTT->GetBinContent(i);
    double bin = histTT->GetBinCenter(i);   
    double fit = res->evaluate(bin);
    cout <<  exp << "   " << fit << endl;
  }
*/
  canv->Update();
  canv->Print("Plots/png/2016_UL_TT_SingleB_combined_Mbb_model.png");
  canv->Print("Plots/pdf/2016_UL_TT_SingleB_combined_Mbb_model.pdf");
  }
