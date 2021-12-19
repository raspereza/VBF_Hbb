#include "Common.h"

using namespace RooFit;

std::ofstream textout("data_chv.txt");

void CreatePDF (int iCAT,
		int iORDER,
		TNtuple * tree,
                TNtuple * tree_tt,
                TNtuple * tree_zj,
                TNtuple * tree_wj,
                TNtuple * tree_st,
                TNtuple * tree_vbf,
                TNtuple * tree_ggh,
		RooWorkspace * w) {

  TH1::SetDefaultSumw2(true);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_"+names[iCAT];
  TString nameHistSubtr = "mbbSubtr_"+names[iCAT];

  TString nameHist_tt = "mbb_tt_"+names[iCAT];
  TString nameHist_zj = "mbb_zj_"+names[iCAT];
  TString nameHist_wj = "mbb_wj_"+names[iCAT];
  TString nameHist_st = "mbb_st_"+names[iCAT];
  TString nameHist_vbf = "mbb_vbf_"+names[iCAT];
  TString nameHist_ggh = "mbb_ggh_"+names[iCAT];

  TH1D * histData = new TH1D(nameHist,"",Nbins,xmin,xmax);
  TH1D * hist = new TH1D(nameHistSubtr,"",NbinsBkg,xmin,xmax);

  TH1D * hist_tt = new TH1D(nameHist_tt,"",NbinsBkg,xmin,xmax);
  TH1D * hist_zj = new TH1D(nameHist_zj,"",NbinsBkg,xmin,xmax);
  TH1D * hist_st = new TH1D(nameHist_st,"",NbinsBkg,xmin,xmax);
  TH1D * hist_wj = new TH1D(nameHist_wj,"",NbinsBkg,xmin,xmax);
  TH1D * hist_vbf = new TH1D(nameHist_vbf,"",NbinsBkg,xmin,xmax);
  TH1D * hist_ggh = new TH1D(nameHist_ggh,"",NbinsBkg,xmin,xmax);

  tree->Draw("mbb_reg>>"+nameHist,cuts[iCAT]);
  tree->Draw("mbb_reg>>"+nameHistSubtr,cuts[iCAT]);
  tree_tt->Draw("mbb_reg>>"+nameHist_tt,"weight*("+cuts[iCAT]+")");
  tree_zj->Draw("mbb_reg>>"+nameHist_zj,"weight*("+cuts[iCAT]+")");
  tree_st->Draw("mbb_reg>>"+nameHist_st,"weight*("+cuts[iCAT]+")");
  tree_wj->Draw("mbb_reg>>"+nameHist_wj,"weight*("+cuts[iCAT]+")");
  tree_vbf->Draw("mbb_reg>>"+nameHist_vbf,"weight*("+cuts[iCAT]+")");
  tree_ggh->Draw("mbb_reg>>"+nameHist_ggh,"weight*("+cuts[iCAT]+")");

  hist->Add(hist_tt,-1);
  hist->Add(hist_zj,-1);
  hist->Add(hist_st,-1);
  hist->Add(hist_wj,-1);
  hist->Add(hist_vbf,-1);
  hist->Add(hist_ggh,-1);

  delete dummy;

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);
  RooRealVar b0("b0_"+names[iCAT],"b0",-1.0,1.0);
  RooRealVar b1("b1_"+names[iCAT],"b1",-1.0,1.0);
  RooRealVar b2("b2_"+names[iCAT],"b2",-1.0,1.0);
  RooRealVar b3("b3_"+names[iCAT],"b3",-1.0,1.0);
  RooRealVar b4("b4_"+names[iCAT],"b4",-1.0,1.0);
  RooRealVar b5("b5_"+names[iCAT],"b5",-1.0,1.0);
  b0.setConstant(false);
  b1.setConstant(false);
  b2.setConstant(false);
  b3.setConstant(false);
  b4.setConstant(false);
  b5.setConstant(false);

  Float_t yield = hist->GetSumOfWeights();
  RooRealVar qcd_yield("qcd_"+names[iCAT]+"_norm","Yield",yield,0.5*yield,2*yield);

  RooArgList argList(b0,b1);
  if (iORDER>=3)
    argList.add(b2);
  if (iORDER>=4)
    argList.add(b3);
  if (iORDER>=5)
    argList.add(b4);
  if (iORDER>=6)
    argList.add(b5);
  
  RooChebychev CHV("qcd_"+names[iCAT],"qcd_"+names[iCAT],mbb,argList);
  RooDataHist dataSubtr("data_subtr_"+names[iCAT],"data",mbb,hist);
  RooDataHist data("data_"+names[iCAT],"data",mbb,histData);  

  RooFitResult * res = CHV.fitTo(dataSubtr,Save(),SumW2Error(kTRUE));  
  RooChi2Var chi2Roo = RooChi2Var("chi2"+names[iCAT],"chi2",CHV,dataSubtr) ;
  double ndof = NbinsBkg - iORDER;
  double chi2 = chi2Roo.getVal();
  double prob = TMath::Prob(chi2,ndof);
  res->Print();
  textout << endl;
  textout << "+++++++++++++++ " << names[iCAT] << " +++++++++++++++++++" << endl;
  textout << endl;
  textout << "Chi2/ndof = " << chi2 << "/" << ndof << " = " << chi2/ndof << "  prob = " << prob << endl;
  textout << "TOTAL DATA YIELD = " << histData->GetSumOfWeights() << endl;
  textout << "TOTAL QCD  YIELD = " << yield << endl;
  cout << endl;

  w->import(CHV);
  w->import(data);
  w->import(qcd_yield);

}

void CreateDataTemplates_chb() {

  TFile * file = new TFile(dirName+"/mbb_and_bdt_all_Nom.root");
  TNtuple * tree     = (TNtuple*)file->Get("Mass_and_BDT_DATA");
  TNtuple * tree_tt  = (TNtuple*)file->Get("Mass_and_BDT_tt");
  TNtuple * tree_zj  = (TNtuple*)file->Get("Mass_and_BDT_ZJets");
  TNtuple * tree_wj  = (TNtuple*)file->Get("Mass_and_BDT_WJets");
  TNtuple * tree_st  = (TNtuple*)file->Get("Mass_and_BDT_Sandgle_Top");
  TNtuple * tree_vbf = (TNtuple*)file->Get("Mass_and_BDT_VBF_Hbb_Dipol");
  TNtuple * tree_ggh = (TNtuple*)file->Get("Mass_and_BDT_ggF_Hbb");
 
  TFile * fileOutput = new TFile("root_shape/data_singleb_shapes.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","data");


  int iorder[5] = {3,2,2,2,2}; 
  for (int i=0; i<5; ++i) 
    CreatePDF(i,iorder[i],tree,tree_tt,tree_zj,tree_wj,tree_st,tree_vbf,tree_ggh,w);
  
  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
