#include "HttStylesNew.cc"
#include "Common.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooMultiPdf.h"

std::ofstream outtext("data_out.txt");

void CreatePDF(int iCAT, 
	       int iORDER_nom,
	       int iORDER_alt,
	       TNtuple * tree,
	       TNtuple * treeZ,
	       TNtuple * treeVBF,
	       TNtuple * treeGGH,
	       RooWorkspace * w) {

  bool blind = true;

  using namespace RooFit;
  TH1::SetDefaultSumw2(true);
  SetStyle();

  char ORDER[4];
  sprintf(ORDER,"%1i",iORDER_nom);
  TString IORDER(ORDER);


  TString Cuts = cuts[iCAT];
  TString CutsBlind = Cuts + "&&(T_reg_mbb<"+Xmin_blind+"|| T_reg_mbb>"+Xmax_blind+")";

  TH1::SetDefaultSumw2(true);

  TCanvas * dummy = new TCanvas("dummy","",800,700);

  TString nameHist = "mbb_"+names[iCAT];
  TString nameBlindHist = "mbb_blind_"+names[iCAT];
  TString nameHistZ = "mbb_zj_"+names[iCAT];
  //TString nameHistZ_ewk = "mbb_zj_ewk_"+names[iCAT];  
  TString nameHistH = "mbb_higgs_"+names[iCAT];
  TString nameHistGGH = "mbb_ggh_"+names[iCAT];
  TString nameHistData = "mbb_data_"+names[iCAT];

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  TH1D * hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  TH1D * histBlind = new TH1D(nameBlindHist,"",NbinsBkg,xmin,xmax);
  TH1D * histZ = new TH1D(nameHistZ,"",NbinsSig,xmin_mc,xmax_mc);
  //TH1D * histZ_ewk = new TH1D(nameHistZ_ewk,"",NbinsSig,xmin_mc,xmax_mc);
  TH1D * histH = new TH1D(nameHistH,"",NbinsSig,xmin_mc,xmax_mc);
  TH1D * histGGH = new TH1D(nameHistGGH,"",NbinsSig,xmin_mc,xmax_mc);
  TH1D * histData = new TH1D(nameHistData,"",Nbins,xmin,xmax);
  TString selection = Form("(%s) * T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_central*T_online_btag_weight", cuts[iCAT].Data());
  treeVBF->Draw(variable + ">>" + nameHistH, selection);
  treeGGH->Draw(variable+">>"+nameHistGGH, selection);

  //treeVBF->Draw(variable+">>"+nameHistH,"T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_central");
  //treeGGH->Draw(variable+">>"+nameHistGGH,"T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_central");
  histH->Scale(luminosity);
  histGGH->Scale(luminosity);
  histH->Add(histH,histGGH,1.,1.);
  
  TString selectionZ = Form("(%s) * T_weight*LUMI*T_HLTweight*T_PUweight*T_V_pTweight_QCD*T_V_pTweight_EWK*T_btag_weight_central*T_online_btag_weight", cuts[iCAT].Data());
  treeZ->Draw(variable+">>"+nameHistZ, selectionZ);
  //treeZ_ewk->Draw(variable+">>"+nameHistZ_ewk,"weight");
  //histZ->Add(histZ,histZ_ewk,1.,1.);
  histZ->Scale(luminosity);

  tree->Draw(variableData+">>"+nameHist,Cuts);
  tree->Draw(variableData+">>"+nameBlindHist,CutsBlind);
  tree->Draw(variableData+">>"+nameHistData,Cuts);
  RooDataHist data_ws("data_"+names[iCAT],"data",mbb,histData);  
  RooDataHist data_ws_blind("data_blind_"+names[iCAT],"data",mbb,histBlind);
  delete dummy;

  InitData(hist);
  InitData(histBlind);
  hist->SetMarkerSize(1.1);
  histBlind->SetMarkerSize(1.1);

  // Z->bb component
  RooRealVar mean_zj("mean_zj","Mean",90,70,210);
  RooRealVar sigma_zj("sigma_zj","Width",10,0,30);
  RooRealVar alpha_zj("alpha_zj","Alpha",0.5,0,2.);
  RooRealVar n_zj("n_zj","n",10,0,20);
    
  RooRealVar b0_zj("b0_zj","b0",0,1);
  RooRealVar b1_zj("b1_zj","b1",0,1);
  RooRealVar b2_zj("b2_zj","b2",0,1);

  RooRealVar f_zj("f_zj","fzj",0,1);
  //

  // Signal component
  RooRealVar mean_sig("mean_sig","Mean",125,80,200);
  RooRealVar sigma_sig("sigma_sig","Width",10,0,30);
  RooRealVar alpha_sig("alpha_sig","Alpha",0.9205);
  RooRealVar n_sig("n_sig","n",2.78,0,10);
    
  RooRealVar b0_sig("b0_sig","b0",0,1);
  RooRealVar b1_sig("b1_sig","b1",0,1);
  RooRealVar b2_sig("b2_sig","b2",0,1);

  RooRealVar f_sig("f_sig","fsig",0,1);
  //

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

   // exp nominal category
  RooRealVar a1_nom("a1_nom_"+names[iCAT],"a1_nom",-1.5,1.5); a1_nom.setVal(0.);
  RooRealVar a2_nom("a2_nom_"+names[iCAT],"a2_nom",-1.5,1.5); a2_nom.setVal(0.);
  RooRealVar a3_nom("a3_nom_"+names[iCAT],"a3_nom",-1.5,1.5); a3_nom.setVal(0.);
  RooRealVar a4_nom("a4_nom_"+names[iCAT],"a4_nom",-1.5,1.5); a4_nom.setVal(0.);
  RooRealVar a5_nom("a5_nom_"+names[iCAT],"a5_nom",-1.5,1.5); a5_nom.setVal(0.);

  RooArgList argListPolExp;

  if (iORDER_nom>=1)
    argListPolExp.add(a1_nom);
  if (iORDER_nom>=2)
    argListPolExp.add(a2_nom);
  if (iORDER_nom>=3)
    argListPolExp.add(a3_nom);
  if (iORDER_nom>=4)
    argListPolExp.add(a4_nom);
  if (iORDER_nom>=5)
    argListPolExp.add(a5_nom);

  RooRealVar a0_alt("a0_alt_"+names[iCAT],"a0_alt",-1.5,1.5);
  RooRealVar a1_alt("a1_alt_"+names[iCAT],"a1_alt",-1.5,1.5);
  RooRealVar a2_alt("a2_alt_"+names[iCAT],"a2_alt",-1.5,1.5);
  RooRealVar a3_alt("a3_alt_"+names[iCAT],"a3_alt",-1.5,1.5);
  RooRealVar a4_alt("a4_alt_"+names[iCAT],"a4_alt",-1.5,1.5);
  RooRealVar a5_alt("a5_alt_"+names[iCAT],"a5_alt",-1.5,1.5);
  RooArgList argList_alt(a0_alt);

  // RooArgList argListPolExp;
  if (iORDER_alt>=2)
    argList_alt.add(a1_alt);
  if (iORDER_alt>=3)
    argList_alt.add(a2_alt);
  if (iORDER_alt>=4)
    argList_alt.add(a3_alt);
  if (iORDER_alt>=5)
    argList_alt.add(a4_alt);
  if (iORDER_alt>=6)
    argList_alt.add(a5_alt);

  RooRealVar tau_decay("tau_decay_"+names[iCAT],"tau_decay",-1.0e-01,-1.0e-06);
  tau_decay.setVal(-9.0e-03);
  argListPolExp.add(tau_decay);

  RooRealVar fbkg("fbkg_"+names[iCAT],"fbkg",0.05,0.,1.);
  RooRealVar fmodel("fmodel_"+names[iCAT],"fmodel",0.01,0.,1.);
  RooRealVar fbkg_alt("fbkg_alt_"+names[iCAT],"fbkg_alt",0.05,0.,1.);
  RooRealVar fmodel_alt("fmodel_alt_"+names[iCAT],"fmodel_alt",0.01,0.,1.);

  RooGenericPdf QCD(Form("qcd_nom_%s", namesCAT[iCAT].c_str()),
                  genericPolynomExp.at(iORDER_nom).c_str(),
                  RooArgSet(mbb,argListPolExp));

  RooGenericPdf QCD_alt(Form("qcd_alt_%s", namesCAT[iCAT].c_str()),genericPolynoms.at(iORDER_alt).c_str(),RooArgSet(mbb,argList_alt));
  //RooChebychev QCD_alt(Form("qcd_alt_%s", namesCAT[iCAT].c_str()),Form("qcd_%s", namesCAT[iCAT].c_str()), mbb,argList_alt);


  RooCategory cat(Form("pdfindex_Tag0_%s", namesCAT[iCAT].c_str()),
                Form("pdfindex_Tag0_%s", namesCAT[iCAT].c_str()));
  RooArgList models;

  RooAddPdf model_bkg(Form("model_bkg_%s", names[iCAT].Data()),"Bkg",RooArgList(model_zj,QCD),fbkg);
  RooAddPdf model_bkg_alt(Form("model_bkg_alt_%s", names[iCAT].Data()),"Bkg",RooArgList(model_zj,QCD_alt),fbkg_alt);
  RooAddPdf Model(Form("Model_%s", names[iCAT].Data()),"Model",RooArgList(model_sig,model_bkg),fmodel);
  RooAddPdf Model_alt(Form("Model_alt_%s", names[iCAT].Data()),"Model",RooArgList(model_sig,model_bkg_alt),fmodel_alt);
  RooDataHist data(Form("data_%s", names[iCAT].Data()),"data",mbb,hist);
  RooDataHist dataBlind("dataBlind","data",mbb,histBlind);
  TH1D * ratioHist = (TH1D*)hist->Clone("ratioHist");
  double chi2_ = 0.0;
  double ndof_ = 0.0;
  RooFitResult * fitRes_0 = QCD.fitTo(data,SumW2Error(kTRUE),Save());
  RooFitResult * fitRes_0_alt = QCD_alt.fitTo(data,SumW2Error(kTRUE),Save());
  fitRes_0->Print();
  fitRes_0_alt->Print();
  RooFitResult * fitRes = Model.fitTo(data,SumW2Error(kTRUE),Save());
  RooFitResult * fitRes_alt = Model_alt.fitTo(data,SumW2Error(kTRUE),Save());

  RooAbsReal* intOfModel = Model.createIntegral(mbb);
  RooAbsReal* intOfQCD = QCD.createIntegral(mbb);
  RooAbsReal* intOfSig = model_sig.createIntegral(mbb);
  RooAbsReal* intOfBkg = model_bkg.createIntegral(mbb);
  RooAbsReal* intOfZbb = model_zj.createIntegral(mbb);

  RooAbsReal* intOfModel_alt = Model_alt.createIntegral(mbb);
  RooAbsReal* intOfQCD_alt = QCD_alt.createIntegral(mbb);
  RooAbsReal* intOfBkg_alt = model_bkg_alt.createIntegral(mbb);

  double normModel = intOfModel->getVal();
  double normQCD = intOfQCD->getVal();
  double normSig = intOfSig->getVal();
  double normBkg = intOfBkg->getVal();
  double normZbb = intOfZbb->getVal();

  double normModel_alt = intOfModel_alt->getVal();
  double normQCD_alt = intOfQCD_alt->getVal();
  double normBkg_alt = intOfBkg_alt->getVal();

  double frac_QCD = (1.0-fmodel.getVal())*(1-fbkg.getVal())/normQCD;
  double frac_QCD_alt = (1.0-fmodel_alt.getVal())*(1-fbkg_alt.getVal())/normQCD_alt;

  double norm = 0;
  for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
    norm += hist->GetBinContent(iBin);
  }

  TH1D * ratioHist_alt = (TH1D*)hist->Clone("ratioHist_alt");
  double chi2_alt_sb = 0.0;
  double ndof_alt_sb = 0.0;

  for (int iBin = 1; iBin<=NbinsBkg; ++iBin) {
    double xCenter = hist->GetBinCenter(iBin);
    double xHist   = hist->GetBinContent(iBin);
    double xError  = hist->GetBinError(iBin);
    double xlower = hist->GetBinLowEdge(iBin);
    double xupper = hist->GetBinLowEdge(iBin+1);
    mbb.setRange("range_mbb",xlower,xupper);

    double xModel  = norm*Model.createIntegral(mbb,"range_mbb")->getVal()/normModel;
    double xQCD    = norm*QCD.createIntegral(mbb,"range_mbb")->getVal()*frac_QCD;

    double xModel_alt = norm*Model_alt.createIntegral(mbb,"range_mbb")->getVal()/normModel_alt;
    double xQCD_alt_bin = norm*QCD_alt.createIntegral(mbb,"range_mbb")->getVal()*frac_QCD_alt;

    if (xError > 0.) {
      double term = (xHist-xModel)/xError;
      double term_alt = (xHist-xModel_alt)/xError;

      if (blind) {
        if (xCenter>xmin_blind&&xCenter<xmax_blind) {
          ratioHist->SetBinContent(iBin,-1.);
          ratioHist->SetBinError(iBin,0.);
          ratioHist_alt->SetBinContent(iBin,-1.);
          ratioHist_alt->SetBinError(iBin,0.);
        }
        else {
          chi2_ += term*term;
          ndof_ += 1.;
          chi2_alt_sb += term_alt*term_alt;
          ndof_alt_sb += 1.;
        }
      }
      else {
        chi2_ += term*term;
        ndof_ += 1.;
        chi2_alt_sb += term_alt*term_alt;
        ndof_alt_sb += 1.;
      }
    }

    if (xQCD > 0.) {
      ratioHist->SetBinContent(iBin,xHist/xQCD);
      ratioHist->SetBinError(iBin,xError/xQCD);
    } else {
      ratioHist->SetBinContent(iBin,0.);
      ratioHist->SetBinError(iBin,0.);
    }

    if (xQCD_alt_bin > 0.) {
      ratioHist_alt->SetBinContent(iBin,xHist/xQCD_alt_bin);
      ratioHist_alt->SetBinError(iBin,xError/xQCD_alt_bin);
    } else {
      ratioHist_alt->SetBinContent(iBin,0.);
      ratioHist_alt->SetBinError(iBin,0.);
    }
  }

  RooChi2Var chi2Roo(Form("chi2_%s", names[iCAT].Data()),"chi2",Model,data);
  RooChi2Var chi2Roo_alt(Form("chi2_alt_%s", names[iCAT].Data()),"chi2_alt",Model_alt,data);
  double chi2 = chi2Roo.getVal();
  double chi2_alt = chi2Roo_alt.getVal();

  double ndof  = NbinsBkg - 1 - iORDER_nom;
  double ndof_alt  = NbinsBkg - 1 - iORDER_alt;
  ndof_ = ndof_ - iORDER_nom - 1.;
  ndof_alt_sb = ndof_alt_sb - iORDER_alt - 1.;
  double prob  = TMath::Prob(chi2,ndof);
  double prob_ = TMath::Prob(chi2_,ndof_);
  double prob_alt = TMath::Prob(chi2_alt,ndof_alt);
  double prob_alt_sb = TMath::Prob(chi2_alt_sb,ndof_alt_sb);

  outtext << endl;
  outtext << "+++++++++++ " << names[iCAT] << " +++++++++++++" << endl;
  outtext << " Cut : " << cuts[iCAT] << endl;
  outtext << endl;
  outtext << "[EXP]   Fitted range : Chi2/ndof = " << chi2_ << "/" << ndof_ << " = " << chi2_/ndof_ << "  prob = " << prob_ << endl;
  outtext << "[EXP]   Full range   : Chi2/ndof = " << chi2 << "/" << ndof << " = " << chi2/ndof << "  prob = " << prob << endl;
  outtext << "[CHEBY] Fitted range : Chi2/ndof = " << chi2_alt_sb << "/" << ndof_alt_sb << " = " << chi2_alt_sb/ndof_alt_sb << "  prob = " << prob_alt_sb << endl;
  outtext << "[CHEBY] Full range   : Chi2/ndof = " << chi2_alt << "/" << ndof_alt << " = " << chi2_alt/ndof_alt << "  prob = " << prob_alt << endl;
  outtext << endl;

  double totalYield = hist->GetSumOfWeights();
  double xBkg = totalYield*(1.0-fmodel.getVal());
  double xQCD = totalYield*(1.0-fmodel.getVal())*(1-fbkg.getVal());
  double xZbb = totalYield*(1.0-fmodel.getVal())*fbkg.getVal();
  double xSig = totalYield*fmodel.getVal();

  double xBkg_alt = totalYield*(1.0-fmodel_alt.getVal());
  double xQCD_alt = totalYield*(1.0-fmodel_alt.getVal())*(1-fbkg_alt.getVal());
  double xZbb_alt = totalYield*(1.0-fmodel_alt.getVal())*fbkg_alt.getVal();
  double xSig_alt = totalYield*fmodel_alt.getVal();

  outtext << "[EXP]   integral (QCD)   = " << xQCD << endl;
  outtext << "[EXP]   integral (Zbb)   = " << xZbb << endl;
  outtext << "[EXP]   integral (Bkg)   = " << xBkg << endl;
  outtext << "[EXP]   integral (Sig)   = " << xSig << endl;
  outtext << endl;
  outtext << "[CHEBY] integral (QCD)   = " << xQCD_alt << endl;
  outtext << "[CHEBY] integral (Zbb)   = " << xZbb_alt << endl;
  outtext << "[CHEBY] integral (Bkg)   = " << xBkg_alt << endl;
  outtext << "[CHEBY] integral (Sig)   = " << xSig_alt << endl;
  outtext << endl;
  outtext << "TOTAL DATA YIELD = " << hist->GetSumOfWeights() << endl;
  outtext << endl;

  RooRealVar qcd_yield_alt(Form("qcd_alt_%s_norm", names[iCAT].Data()),"Yield",xQCD_alt,0.5*xQCD_alt,2*xQCD_alt);
  a1_nom.setConstant(false);
  a2_nom.setConstant(false);
  a3_nom.setConstant(false);
  a4_nom.setConstant(false);
  a5_nom.setConstant(false);
  tau_decay.setConstant(false);


 models.add(QCD);
  models.add(QCD_alt);

//  models.add(model_xap2_bkg);
  RooMultiPdf multipdf(Form("multipdf_Tag0_%s", namesCAT[iCAT].c_str()),
                     "MultiPdf for Tag0", cat, models);

  RooRealVar qcd_yield(Form("multipdf_Tag0_%s_norm", namesCAT[iCAT].c_str()),
                     "Yield", xQCD, 0.5*xQCD, 2*xQCD);

  w->import(multipdf);

//  w->import(QCD);
  w->import(data_ws);
  w->import(data_ws_blind);
  w->import(qcd_yield);
  w->import(qcd_yield_alt);
}

void CreateDataTemplates_exp_multipdf() {

	TFile* file_data = new TFile(dirName + "/" + data_file_name);
TFile* file_zjet = new TFile(dirName+"/"+FileNamesZJetDNN[0]);
TFile* file_vbfh = new TFile(dirName+"/"+FileNamesVBFDNN[0]);
TFile* file_ggh  = new TFile(dirName+"/"+FileNamesGGHDNN[0]);
  TNtuple * tree = (TNtuple*)file_data->Get("tree");
  TNtuple * tree_zj = (TNtuple*)file_zjet->Get("tree");
  //TNtuple * treeZ_ewk = (TNtuple*)file->Get("tree");
  TNtuple * tree_vbf = (TNtuple*)file_vbfh->Get("tree");
  TNtuple * tree_ggh = (TNtuple*)file_ggh->Get("tree");
 
  TFile * fileOutput = new TFile("workspace/data_resolved_vbfhbb_2022.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","data");

  //int iorder_nom[7] = {0,0,0,0,0,0,0}; 
  int iorder_nom[7] = {0,1,0,0,0,1,0};
  //int iorder_alt[7] = {2,2,2,2,2,2,2};
  //int iorder_alt[7] = {1,1,1,1,1,1,1};
  int iorder_alt[7] = {1,2,2,2,2,2,2};
  for (int i=0; i<7; ++i) 
    CreatePDF(i,iorder_nom[i],iorder_alt[i],tree,tree_zj,tree_vbf,tree_ggh,w);
  
  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
