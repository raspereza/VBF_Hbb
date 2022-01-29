#include "HttStylesNew.cc"
#include "Common.h"

std::ofstream outtext("data_out.txt");

void CreatePDF(int iCAT, 
	       int iORDER,
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
  sprintf(ORDER,"%1i",iORDER);
  TString IORDER(ORDER);

  vector<TString> sample_names = {
    //    "WJets",
    //    "ZJets",
    //    "tt",
    //    "Sandgle_Top",
    //    "VBF_Hbb_Dipol",
    //    "ggF_Hbb_POWHEG"
  };

  TString Cuts = cuts[iCAT];
  TString CutsBlind = Cuts + "&&("+variable+"<"+Xmin_blind+"||"+variable+">"+Xmax_blind+")";

  TH1::SetDefaultSumw2(true);

  TCanvas * dummy = new TCanvas("dummy","",800,700);

  TString nameHist = "mbb_"+names[iCAT];
  TString nameBlindHist = "mbb_blind_"+names[iCAT];
  TString nameHistZ = "mbb_zj_"+names[iCAT];
  TString nameHistH = "mbb_higgs_"+names[iCAT];
  TString nameHistGGH = "mbb_ggh_"+names[iCAT];
  TString nameHistData = "mbb_data_"+names[iCAT];

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  TH1D * hist = new TH1D(nameHist,"",NbinsBkg,xmin,xmax);
  TH1D * histBlind = new TH1D(nameBlindHist,"",NbinsBkg,xmin,xmax);
  TH1D * histZ = new TH1D(nameHistZ,"",NbinsSig,xmin_mc,xmax_mc);
  TH1D * histH = new TH1D(nameHistH,"",NbinsSig,xmin_mc,xmax_mc);
  TH1D * histGGH = new TH1D(nameHistGGH,"",NbinsSig,xmin_mc,xmax_mc);
  TH1D * histData = new TH1D(nameHistData,"",Nbins,xmin,xmax);


  treeVBF->Draw(variable+">>"+nameHistH,"weight");
  treeGGH->Draw(variable+">>"+nameHistGGH,"weight");
  histH->Add(histH,histGGH,1.,1.);

  treeZ->Draw(variable+">>"+nameHistZ,"weight");
  tree->Draw(variable+">>"+nameHist,Cuts);
  tree->Draw(variable+">>"+nameBlindHist,CutsBlind);
  tree->Draw(variable+">>"+nameHistData,Cuts);
  RooDataHist data_ws("data_"+names[iCAT],"data",mbb,histData);  

  delete dummy;

  InitData(hist);
  InitData(histBlind);
  hist->SetMarkerSize(1.1);
  histBlind->SetMarkerSize(1.1);

  // Z->bb component
  RooRealVar mean_zj("mean_zj","Mean",90,70,210);
  RooRealVar sigma_zj("sigma_zj","Width",10,0,30);
  RooRealVar alpha_zj("alpha_zj","Alpha",1,0,20);
  RooRealVar n_zj("n_zj","n",2,0,20);
    
  RooRealVar b0_zj("b0_zj","b0",0.5,0,1);
  RooRealVar b1_zj("b1_zj","b1",0.5,0,1);
  RooRealVar b2_zj("b2_zj","b2",0.5,0,1);

  RooRealVar f_zj("f_zj","fzj",0.5,0,1);
  //

  // Signal component
  RooRealVar mean_sig("mean_sig","Mean",125,70,210);
  RooRealVar sigma_sig("sigma_sig","Width",10,0,30);
  RooRealVar alpha_sig("alpha_sig","Alpha",1,0,20);
  RooRealVar n_sig("n_sig","n",2,0,20);
    
  RooRealVar b0_sig("b0_sig","b0",0.5,0,1);
  RooRealVar b1_sig("b1_sig","b1",0.5,0,1);
  RooRealVar b2_sig("b2_sig","b2",0.5,0,1);

  RooRealVar f_sig("f_sig","fsig",0.5,0,1);
  //

  RooRealVar a1("a1_"+names[iCAT],"a1",-0.2,0.2); a1.setVal(0.);
  RooRealVar a2("a2_"+names[iCAT],"a2",-0.2,0.2); a2.setVal(0.);
  RooRealVar a3("a3_"+names[iCAT],"a3",-0.2,0.2); a3.setVal(0.);
  RooRealVar a4("a4_"+names[iCAT],"a4",-0.2,0.2); a4.setVal(0.);
  RooRealVar a5("a5_"+names[iCAT],"a5",-0.2,0.2); a5.setVal(0.);

  RooArgList argListPolExp;
  
  if (iORDER>=1)
    argListPolExp.add(a1);
  if (iORDER>=2)
    argListPolExp.add(a2);
  if (iORDER>=3)
    argListPolExp.add(a3);
  if (iORDER>=4)
    argListPolExp.add(a4);
  if (iORDER>=5)
    argListPolExp.add(a5);


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

  RooRealVar tau_decay("tau_decay_"+names[iCAT],"tau_decay",-1.0e-02,-5.0e-03);
  tau_decay.setVal(-9.0e-03);
  argListPolExp.add(tau_decay);

  RooRealVar fbkg("fbkg_"+names[iCAT],"fbkg",0.05,0.,1.);
  RooRealVar fmodel("fmodel_"+names[iCAT],"fmodel",0.01,0.,1.);
  RooRealVar fexp("fexp_"+names[iCAT],"fexp",0.5,0.,1.);

  RooGenericPdf QCD((string("qcd_")+namesCAT[iCAT]).c_str(),genericPolynomExp.at(iORDER).c_str(),RooArgSet(mbb,argListPolExp));
  RooAddPdf model_bkg("model_bkg_"+names[iCAT],"Bkg",RooArgList(model_zj,QCD),fbkg);
  RooAddPdf Model("Model_"+names[iCAT],"Model",RooArgList(model_sig,model_bkg),fmodel);
  RooDataHist data("data_"+names[iCAT],"data",mbb,hist);
  RooDataHist dataBlind("dataBlind","data",mbb,histBlind);
  TH1D * ratioHist = (TH1D*)hist->Clone("ratioHist");
  double chi2_ = 0.0;
  double ndof_ = 0.0;
  RooFitResult * fitRes_0 = QCD.fitTo(data,SumW2Error(kTRUE),Save());
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
  RooChi2Var chi2Roo =  RooChi2Var("chi2"+names[iCAT],"chi2",Model,data) ;
  double chi2 = chi2Roo.getVal();

  double ndof  = NbinsBkg - 1 -iORDER;
  ndof_ = ndof_ - iORDER - 1.;
  double prob  = TMath::Prob(chi2,ndof);
  double prob_ = TMath::Prob(chi2_,ndof_);
  outtext << endl;
  outtext << "+++++++++++ " << names[iCAT] << " +++++++++++++" << endl;
  outtext << " Cut : " << cuts[iCAT] << endl;
  outtext << endl;
  outtext << "Fitted range : Chi2/ndof = " << chi2_ << "/" << ndof_ << " = " << chi2_/ndof_ << "  prob = " << prob_ << endl;
  outtext << "Full range   : Chi2/ndof = " << chi2 << "/" << ndof << " = " << chi2/ndof << "  prob = " << prob << endl;
  outtext << endl;
  double totalYield = hist->GetSumOfWeights();
  double xBkg = totalYield*(1.0-fmodel.getVal());
  double xQCD = totalYield*(1.0-fmodel.getVal())*(1-fbkg.getVal());
  double xZbb = totalYield*(1.0-fmodel.getVal())*fbkg.getVal();
  double xSig = totalYield*fmodel.getVal();
  outtext << "integral (QCD)   = " << xQCD << endl;
  outtext << "integral (Zbb)   = " << xZbb << endl;
  outtext << "integral (Bkg)   = " << xBkg << endl;
  outtext << "integral (Sig)   = " << xSig << endl;
  outtext << endl;
  outtext << "TOTAL DATA YIELD = " << hist->GetSumOfWeights() << endl;
  outtext << endl;
  RooRealVar qcd_yield("qcd_"+names[iCAT]+"_norm","Yield",xQCD,0.5*xQCD,2*xQCD);

  w->import(QCD);
  w->import(data_ws);
  w->import(qcd_yield);

}

void CreateDataTemplates_exp() {
  
  TFile * file = new TFile(dirName+"/mbb_and_bdt_all_Nom.root");
  TNtuple * tree = (TNtuple*)file->Get("Mass_and_BDT_DATA");
  TNtuple * tree_zj = (TNtuple*)file->Get("Mass_and_BDT_ZJets");
  TNtuple * tree_vbf = (TNtuple*)file->Get("Mass_and_BDT_VBF_Hbb_Dipol");
  TNtuple * tree_ggh = (TNtuple*)file->Get("Mass_and_BDT_ggF_Hbb_POWHEG");
 
  TFile * fileOutput = new TFile("root_shape/data_singleb_shapes.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","data");

  int iorder[4] = {2,1,0,0}; 
  for (int i=0; i<4; ++i) 
    CreatePDF(i,iorder[i],tree,tree_zj,tree_vbf,tree_ggh,w);
  
  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();


}
