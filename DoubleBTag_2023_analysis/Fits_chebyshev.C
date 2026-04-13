#include "HttStylesNew.cc"
#include "Common.h"
//root -l -b -q -e 'gROOT->ProcessLine(".L Fits_chebyshev.C"); RunAllFits(true);'
void Fits_chebyshev(int iCAT=0, 
	  int iORDER=2,
	  bool blind=true,
	  double ymin_ratio = 0.69,
	  double ymax_ratio = 1.31) {

  using namespace RooFit;
  TH1::SetDefaultSumw2(true);
  SetStyle();
  double lumi = 1.0;
  char ORDER[4];
  sprintf(ORDER,"%1i",iORDER);
  TString IORDER(ORDER);

  TString blinded("unblinded");
  if (blind) blinded = "blinded";
  vector<TString> sample_names = {
    //    "WJets",
    //    "ZJets",
    //    "tt",
    //    "Sandgle_Top",
    //    "VBF_Hbb_Dipol",
    //    "ggF_Hbb_POWHEG"
  };
  TFile* file_data = new TFile(dirName + "/" + data_file_name);
  TFile* file_z = new TFile(dirName+"/"+FileNamesZJetDNN[0]);
  TFile* file_vbf = new TFile(dirName+"/"+FileNamesVBFDNN[0]);
  TFile* file_ggh  = new TFile(dirName+"/"+FileNamesGGHDNN[0]);

  TNtuple * tree = (TNtuple*)file_data->Get("tree");
  TNtuple * treeZ = (TNtuple*)file_z->Get("tree");
  TNtuple * treeVBF = (TNtuple*)file_vbf->Get("tree");
  TNtuple * treeGGH = (TNtuple*)file_ggh->Get("tree");
  //comments at least start to put the files for signals data and zj
  //Then make the trees


  TString Cuts = cuts[iCAT];
  TString CutsBlind = Cuts + "&&("+"T_reg_mbb"+"<"+xmin_blind+"||"+"T_reg_mbb"+">"+xmax_blind+")";

  TH1::SetDefaultSumw2(true);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_";
  TString nameBlindHist = "mbb_blind_";
  TString nameHistZ = "mbb_zj";
  TString nameHistH = "mbb_higgs";
  TString nameHistGGH = "mbb_ggh";
  TString nameHistZ_cat = "mbb_zj_cat";
  TString nameHistH_cat = "mbb_higgs_cat";
  TString nameHistGGH_cat = "mbb_ggh_cat";

  TH1D * hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  TH1D * histBlind = new TH1D(nameBlindHist,"",NbinsBkg,xmin,xmax);
  TH1D * histZ = new TH1D(nameHistZ,"",NbinsSig,xmin_mc,xmax_mc);
  TH1D * histH = new TH1D(nameHistH,"",NbinsSig,xmin_mc,xmax_mc);
  TH1D * histGGH = new TH1D(nameHistGGH,"",NbinsSig,xmin_mc,xmax_mc);

  TH1D * histZ_cat = new TH1D(nameHistZ_cat,"",NbinsSig,xmin_mc,xmax_mc);
  TH1D * histH_cat = new TH1D(nameHistH_cat,"",NbinsSig,xmin_mc,xmax_mc);
  TH1D * histGGH_cat = new TH1D(nameHistGGH_cat,"",NbinsSig,xmin_mc,xmax_mc);

  //use the same weights that you use for signal
  TString selection = Form("(%s) * T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_multiWP*T_online_btag_weight", cuts[iCAT].Data());
  treeVBF->Draw(variable + ">>" + nameHistH_cat, selection);
  treeGGH->Draw(variable+">>"+nameHistGGH_cat, selection);
  histH_cat->Add(histH_cat,histGGH_cat,1.,1.);
  histH_cat->Scale(lumi);
  treeVBF->Draw(variable + ">>" + nameHistH, "T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_multiWP*T_online_btag_weight");
  treeGGH->Draw(variable+">>"+nameHistGGH, "T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_multiWP*T_online_btag_weight");
  histH->Add(histH,histGGH,1.,1.);
  histH->Scale(lumi);
  Float_t norm_frac_higgs = histH_cat->Integral()/histH->Integral();
  std::cout << "Total Higgs: " << histH->Integral() << " Cat Higgs: " << histH_cat->Integral() << " Norm: " << norm_frac_higgs << std::endl ;
  histH->Scale(norm_frac_higgs);

  TString selectionZ = Form("(%s) * T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_multiWP*T_online_btag_weight*T_V_pTweight_QCD*T_V_pTweight_EWK", cuts[iCAT].Data());
  treeZ->Draw(variable + ">>" + nameHistZ_cat, selectionZ);
  treeZ->Draw(variable + ">>" + nameHistZ,"T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_multiWP*T_online_btag_weight*T_V_pTweight_QCD*T_V_pTweight_EWK");
  Float_t norm_frac_z = histZ_cat->Integral()/histZ->Integral();
  std::cout << "Total Z+Jets: " << histZ->Integral() << " Cat Z+Jets: " << histZ_cat->Integral() << " Norm: " << norm_frac_z << std::endl ;
  histZ->Scale(norm_frac_z);
  histZ->Scale(lumi);
  std::cout << "Z fit events: ********************: " <<  histZ->Integral() <<  std::endl;
  tree->Draw("T_reg_mbb>>"+nameHist,Cuts);
  tree->Draw("T_reg_mbb>>"+nameBlindHist,CutsBlind);
  double yield_ini = hist->GetSumOfWeights();

  /*
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
    hist->Add(hist_sample,-1.);RooChebychev

RooChebychev


e
e
    histBlind->Add(histBlind_sample,-1.);
  }
  */
  
  delete dummy;

  InitData(hist);
  InitData(histBlind);
  hist->SetMarkerSize(1.1);
  histBlind->SetMarkerSize(1.1);
  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  // Z->bb component
  RooRealVar mean_zj("mean_zj","Mean",90,80,200);
  RooRealVar sigma_zj("sigma_zj","Width",10,0,30);
  RooRealVar alpha_zj("alpha_zj","Alpha",0.5,0,2.);
  RooRealVar n_zj("n_zj","n",2,0,20);
    
  RooRealVar b0_zj("b0_zj","b0",0,1);
  RooRealVar b1_zj("b1_zj","b1",0,1);
  RooRealVar b2_zj("b2_zj","b2",0,1);

  RooRealVar f_zj("f_zj","fzj",0.5,0,1);
  //

  // Signal component
  RooRealVar mean_sig("mean_sig","Mean",125,80,200);
  RooRealVar sigma_sig("sigma_sig","Width",10,0,30);
  RooRealVar alpha_sig("alpha_sig","Alpha",0.5,0,2.);
  RooRealVar n_sig("n_sig","n",2.78,0,10);
    
  RooRealVar b0_sig("b0_sig","b0",0,1);
  RooRealVar b1_sig("b1_sig","b1",0,1);
  RooRealVar b2_sig("b2_sig","b2",0,1);

  RooRealVar f_sig("f_sig","fsig",0.5,0,1);
  //
/*
  RooRealVar a1("a1_"+names[iCAT],"a1",-0.2,0.2); a1.setVal(0.);
  RooRealVar a2("a2_"+names[iCAT],"a2",-0.2,0.2); a2.setVal(0.);
  RooRealVar a3("a3_"+names[iCAT],"a3",-0.2,0.2); a3.setVal(0.);
  RooRealVar a4("a4_"+names[iCAT],"a4",-0.2,0.2); a4.setVal(0.);
  RooRealVar a5("a5_"+names[iCAT],"a5",-0.2,0.2); a5.setVal(0.);
*/
  RooRealVar a0("a0_"+names[iCAT],"a0",-1.0,1.0);
  RooRealVar a1("a1_"+names[iCAT],"a1",-1.0,1.0);
  RooRealVar a2("a2_"+names[iCAT],"a2",-1.0,1.0);
  RooRealVar a3("a3_"+names[iCAT],"a3",-1.0,1.0);
  RooRealVar a4("a4_"+names[iCAT],"a4",-1.0,1.0);
  RooRealVar a5("a5_"+names[iCAT],"a5",-1.0,1.0);
  RooArgList argList(a0);

  // RooArgList argListPolExp;
if (iORDER>=2)
    argList.add(a1);
  if (iORDER>=3)
    argList.add(a2);
  if (iORDER>=4)
    argList.add(a3);
  if (iORDER>=5)
    argList.add(a4);
  if (iORDER>=6)
    argList.add(a5);

  // ZJets
  RooBernstein BRN_zj("brn_zj","Bernstein",mbb,RooArgList(b0_zj,b1_zj,b2_zj));
  RooCBShape cb_zj("cb_zj","CBshape",mbb,mean_zj,sigma_zj,alpha_zj,n_zj);
  RooAddPdf model_zj("model_zj","ZJets",RooArgList(cb_zj,BRN_zj),f_zj);

  RooDataHist data_zj("data_zj","data_zj",mbb,histZ);
  RooFitResult * res_zj = model_zj.fitTo(data_zj,Save(),SumW2Error(kTRUE));
  res_zj->Print();
  mean_zj.setConstant(true);
  sigma_zj.setConstant(true);
  alpha_zj.setConstant(true);
  n_zj.setConstant(true);
  b0_zj.setConstant(true);
  b1_zj.setConstant(true);
  b2_zj.setConstant(true);
  f_zj.setConstant(true);

  // Higgs
  RooBernstein BRN_sig("brn_sig","Bernstein",mbb,RooArgList(b0_sig,b1_sig,b2_sig));
  RooCBShape cb_sig("cb_sig","CBshape",mbb,mean_sig,sigma_sig,alpha_sig,n_sig);
  RooAddPdf model_sig("model_sig","ZJets",RooArgList(cb_sig,BRN_sig),f_sig);

  RooDataHist data_sig("data_sig","data_sig",mbb,histH);
  RooFitResult * res_sig = model_sig.fitTo(data_sig,Save(),SumW2Error(kTRUE));
  res_sig->Print();
  mean_sig.setConstant(true);
  sigma_sig.setConstant(true);
  alpha_sig.setConstant(true);
  n_sig.setConstant(true);
  b0_sig.setConstant(true);
  b1_sig.setConstant(true);
  b2_sig.setConstant(true);
  f_sig.setConstant(true);


  RooRealVar fbkg("fbkg_"+names[iCAT],"fbkg",0.05,0.,1.);
  RooRealVar fmodel("fmodel_"+names[iCAT],"fmodel",0.01,0.,1.);
  RooRealVar fexp("fexp_"+names[iCAT],"fexp",0.5,0.,1.);

  //RooGenericPdf QCD(polynomNames.at(iORDER).c_str(),genericPolynomExp.at(iORDER).c_str(),RooArgSet(mbb,argListPolExp));
  RooChebychev QCD("qcd_"+names[iCAT],"qcd_"+names[iCAT],mbb,argList);
  //  RooExponential QCD("exp2_"+names[iCAT],"Exp",mbb,tau_decay);
  //  RooProdPdf QCD("qcd_"+names[iCAT],"Exp",RooArgSet(tf,QCD_exp));
  RooAddPdf model_bkg("model_bkg_"+names[iCAT],"Bkg",RooArgList(model_zj,QCD),fbkg);
  RooAddPdf Model("Model_"+names[iCAT],"Model",RooArgList(model_sig,model_bkg),fmodel);
  RooDataHist data("data_"+names[iCAT],"data",mbb,hist);
  RooDataHist dataBlind("dataBlind","data",mbb,histBlind);
  TH1D * ratioHist = (TH1D*)hist->Clone("ratioHist");
 double chi2_ = 0.0;
  double ndof_ = 0.0;
  RooFitResult * fitRes_0 = QCD.fitTo(data,SumW2Error(kTRUE),Save());
  cout << endl;
  fitRes_0->Print();
  RooFitResult * fitRes = Model.fitTo(data,SumW2Error(kTRUE),Save());
  RooAbsReal* intOfModel = Model.createIntegral(mbb);
  RooAbsReal* intOfQCD = QCD.createIntegral(mbb);
  RooAbsReal* intOfSig = model_sig.createIntegral(mbb);
  RooAbsReal* intOfBkg = model_bkg.createIntegral(mbb);
  RooAbsReal* intOfZbb = model_zj.createIntegral(mbb);
  double normModel = intOfModel->getVal();
  double normQCD = intOfQCD->getVal();
  double normSig = intOfSig->getVal();
  double normBkg = intOfBkg->getVal();
  double normZbb = intOfZbb->getVal();
  double frac_Model = 1.0/normModel;
  double frac_QCD = (1.0-fmodel.getVal())*(1-fbkg.getVal())/normQCD;
  double frac_Zbb = (1.0-fmodel.getVal())*fbkg.getVal()/normZbb;
  double frac_Sig   = fmodel.getVal()/normSig;
  double norm = 0;
  for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
    norm += hist->GetBinContent(iBin);
  }

  for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
    double xCenter = hist->GetBinCenter(iBin);
    double xHist   = hist->GetBinContent(iBin);
    double xError  = hist->GetBinError(iBin);
    double xlower = hist->GetBinLowEdge(iBin);
    double xupper = hist->GetBinLowEdge(iBin+1);
    mbb.setRange("range_mbb",xlower,xupper);    
    double xModel  = norm*Model.createIntegral(mbb,"range_mbb")->getVal()/normModel;
    double xQCD    = norm*QCD.createIntegral(mbb,"range_mbb")->getVal()*frac_QCD;
    double term = (xHist-xModel)/xError;
    double rat = xHist/xQCD;
    double ratError = xError/xQCD;
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
  TH1D * ratioZJetsH = new TH1D("ratioZJetsH","",Nbins,xmin,xmax);
  TH1D * ratioSigH = new TH1D("ratioSigH","",Nbins,xmin,xmax);
  TH1D * ratioModelH = new TH1D("ratioModelH","",Nbins,xmin,xmax);
  for (int iB=1; iB<=Nbins; ++iB) {
    double xCenter = ratioZJetsH->GetBinCenter(iB);
    mbb.setVal(xCenter);
    double xmodel = Model.getVal()*frac_Model;
    double xqcd = QCD.getVal()*frac_QCD;
    double xsig = model_sig.getVal()*frac_Sig;
    double xzj  = model_zj.getVal()*frac_Zbb;
    ratioZJetsH->SetBinContent(iB,1.0+xzj/xqcd);
    ratioSigH->SetBinContent(iB,1.0+xsig/xqcd);
    ratioModelH->SetBinContent(iB,1.0+(xzj+xsig)/xqcd);
  }
  InitSignal(ratioModelH);
  InitSignal(ratioZJetsH);
  InitSignal(ratioSigH);
  ratioModelH->SetLineColor(kRed);
  ratioZJetsH->SetLineColor(kCyan);
  ratioSigH->SetLineColor(kOrange);
  ratioModelH->SetLineStyle(1);
  ratioZJetsH->SetLineStyle(1);
  ratioSigH->SetLineStyle(1);
  ratioModelH->SetLineWidth(2);
  ratioZJetsH->SetLineWidth(2);
  ratioSigH->SetLineWidth(2);

  RooPlot* xframe = mbb.frame();
  if (blind)
    data.plotOn(xframe,LineColor(kGray),LineWidth(2),
            MarkerColor(kGray),MarkerSize(1.0));
  else
    data.plotOn(xframe,MarkerSize(1.1)); 

  //  Model.plotOn(xframe,LineColor(kRed));
  Model.plotOn(xframe,Components(model_bkg),LineColor(kCyan),Name("model_bkg_curve"));
  Model.plotOn(xframe,Components(QCD),LineStyle(kDashed),LineColor(kBlue),Name("qcd_curve"));
  if (blind)
    dataBlind.plotOn(xframe,LineColor(1),LineWidth(2),MarkerColor(1),MarkerSize(1.0), Name("data_blind_hist"));

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
  frame->GetYaxis()->SetTitle("Events/2.0 GeV");

  frameRatio->SetMarkerColor(1);
  frameRatio->SetMarkerStyle(20);
  frameRatio->SetMarkerSize(1.2);
  frameRatio->SetLineColor(1);
  frameRatio->GetYaxis()->SetRangeUser(0.901,1.099);
  frameRatio->GetYaxis()->SetNdivisions(505);
  frameRatio->GetYaxis()->SetTitleOffset(1.6);
  frameRatio->GetXaxis()->SetLabelFont(42);
  //  frameRatio->GetXaxis()->SetTitleFont(22);
  frameRatio->GetXaxis()->SetLabelOffset(0.04);
  frameRatio->GetXaxis()->SetLabelSize(0.14);
  frameRatio->GetXaxis()->SetTitleOffset(1.0);
  frameRatio->GetXaxis()->SetTickLength(0.07);
  frameRatio->GetYaxis()->SetTitle("obs/fit");
  frameRatio->GetYaxis()->SetLabelFont(42);
  frameRatio->GetYaxis()->SetLabelOffset(0.015);
  frameRatio->GetYaxis()->SetLabelSize(0.11);
  frameRatio->GetYaxis()->SetTickLength(0.04);
  frameRatio->GetXaxis()->SetTitle("m_{bb} (GeV)");

  RooChi2Var chi2Roo =  RooChi2Var("chi2"+names[iCAT],"chi2",Model,data) ;
  double chi2 = chi2Roo.getVal();

  double ndof  = NbinsBkg - 1 -iORDER;
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
  double totalYield = hist->GetSumOfWeights();
  double xBkg = totalYield*(1.0-fmodel.getVal());
  double xQCD = totalYield*(1.0-fmodel.getVal())*(1-fbkg.getVal());
  double xZbb = totalYield*(1.0-fmodel.getVal())*fbkg.getVal();
  double xSig = totalYield*fmodel.getVal();
  cout << "integral (QCD)   = " << xQCD << std::endl;
  cout << "integral (Zbb)   = " << xZbb << std::endl;
  cout << "integral (Bkg)   = " << xBkg << std::endl;
  cout << "integral (Sig)   = " << xSig << std::endl;
  cout << endl;
  cout << "TOTAL DATA YIELD = " << hist->GetSumOfWeights() << endl;
  cout << endl;
  char str1[30];
  sprintf(str1,"#chi^{2}/ndof = %4.2f",chi2/ndof);
  char str2[30];
  sprintf(str2,"P(#chi^{2}) = %4.2f",prob);
  char str3[30];
    
  sprintf(str3,"Cheb%1i+Zpeak",iORDER);

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

  TBox *blindBoxUpper = new TBox();
  blindBoxUpper->SetFillColor(kGray);  // very light gray
  blindBoxUpper->SetLineColor(0);
  blindBoxUpper->SetFillStyle(1001);
  frame->Draw();
  if (blind) {
  double yMaxUpper = frame->GetYaxis()->GetXmax();

    blindBoxUpper->DrawBox(
      xmin_blind, 0.0,
      xmax_blind, yMaxUpper
    );
  }
  xframe->Draw("same");
  //  canv->Update();
  TLatex * text3 = new TLatex(0.22,0.3,str3);
  TLatex * text1 = new TLatex(0.22,0.2,str1);
  TLatex * text2 = new TLatex(0.22,0.1,str2);
  text1->SetNDC();
  text2->SetNDC();
  text3->SetNDC();
  text1->SetTextSize(0.05);
  text1->SetTextFont(42);
  text2->SetTextSize(0.05);
  text2->SetTextFont(42);
  text3->SetTextSize(0.05);
  text3->SetTextFont(42);
  text1->Draw();
  text2->Draw();
  text3->Draw();
  char str[20];
  int cat_n = iCAT -1;
  sprintf(str,"%s" ,names[iCAT].Data());
  TLatex * text = new TLatex(0.7,0.84,str);
  text->SetNDC();
  text->SetTextFont(42);
  text->SetTextSize(0.04);
  text->SetTextColor(kBlack);
  text->Draw();
   TLatex *tex_cms = new TLatex(0.57,0.94,"#bf{CMS} #it{Preliminary}                         27.0 fb^{-1} (2023)  ");
   tex_cms->SetNDC();
   tex_cms->SetTextAlign(20);
   tex_cms->SetTextFont(42);
   tex_cms->SetTextSize(0.05);
   tex_cms->SetLineWidth(2);
   tex_cms->Draw();

   TLegend * leg = new TLegend(0.6232092,0.589896,0.8638968,0.807281);
  SetLegendStyle(leg);
  leg->SetTextSize(0.035);
  //leg->AddEntry(histVBF1,"qqH","f");
  //leg->AddEntry(histGGH,"ggH","f");
leg->AddEntry(xframe->findObject("data_blind_hist"), "Data (<104 & > 146)", "epx0");
leg->AddEntry(xframe->findObject("model_bkg_curve"), "QCD + ZJets", "L");
leg->AddEntry(xframe->findObject("qcd_curve"), "QCD", "L");
  leg->Draw();

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
  TBox *blindBoxLower = new TBox();
  blindBoxLower->SetFillColor(kGray);
  blindBoxLower->SetLineColor(0);
  blindBoxLower->SetFillStyle(1001);
  if (blind) {
  blindBoxLower->DrawBox(
    xmin_blind, ymin_ratio,
    xmax_blind, ymax_ratio
  );
  }
  if (blind) {
  blindBoxLower->DrawBox(xmin_blind, ymin_ratio,
                          xmax_blind, ymax_ratio);
  }

  TLine * line = new TLine(xmin,1.,xmax,1.);
  line->SetLineColor(kBlue);
  line->SetLineStyle(2);
  line->SetLineWidth(2);
  ratioZJetsH->Draw("lsame");
  if (!blind) ratioSigH->Draw("hsame");
  if (!blind) ratioModelH->Draw("hsame");
  ratioHist->Draw("e1same");
  line->Draw();


  lower->Modified();
  lower->RedrawAxis();
  canv->cd();
  canv->Modified();
  canv->cd();
  canv->SetSelected(canv);

  /*
   TLatex *tex_cms = new TLatex(0.58,0.96,"#bf{CMS} #it{Preliminary}                      27.0 fb^{-1} (2022)  ");
   tex_cms->SetNDC();
   tex_cms->SetTextAlign(20);
   tex_cms->SetTextFont(42);
   tex_cms->SetTextSize(0.04);
   tex_cms->SetLineWidth(2);
   tex_cms->Draw();
  */

  canv->Update();
  TString dir = "/eos/user/m/mukherje/www/VBFHBB/2023/bkg_fit/";
  canv->Print(dir+"/2023_data_cheb_pol_fit_cat_"+names[iCAT]+"_"+blinded+".png");
  canv->Print(dir+"/2023_data_cheb_pol_fit_cat_"+names[iCAT]+"_"+blinded+".pdf");
  //canv->Print("QCD_exp_pol_"+IORDER+"_"+names[iCAT]+"_"+blinded+".png");

}

void RunAllFits(bool blind=true, double ymin_ratio=0.69, double ymax_ratio=1.31)
{
  // number of categories: use your arrays from Common.h (cuts/names)
  // If you already have something like nCAT in Common.h, use that instead.
  const int nCAT = 7;   // change if you have different number of categories

  // Choose the polynomial order per category (edit as you like)
  // Example: all order-0 everywhere
  int iorder[nCAT] = {2,2,2,2,2,2,2};

  for (int icat = 0; icat < nCAT; ++icat) {
    std::cout << "\n=============================\n";
    std::cout << " Running category " << icat << " (" << names[icat] << "), order=" << iorder[icat] << "\n";
    std::cout << "=============================\n";

    Fits_chebyshev(icat, iorder[icat], blind, ymin_ratio, ymax_ratio);
  }
}
