#include "Common.h"

using namespace RooFit;

void CreateDataTemplates_TF() {

  std::ofstream outtext("data_tf.txt");

  int iORDER = 3; // Order of CHV polynomial for baseline function 
  int porder[5] = {0,2,2,2,1}; // order polynomials of the transfer functions

  TFile * file = new TFile(dirName+"/mbb_and_bdt_all_Nom.root");
  TTree * tree = (TTree*)file->Get("Mass_and_BDT_DATA");

  vector<TNtuple*> tuples;
  vector<TString> sample_names = {
    "tt",
    "ZJets",
    "WJets",
    "Sandgle_Top",
    "VBF_Hbb_Dipol",
    "ggF_Hbb"
  };

  TFile * fileOutput = new TFile("root_shape/data_singleb_shapes.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","data");
  
  TH1::SetDefaultSumw2(true);

  TString nameHist = "mbb_"+names[0];
  TString nameHistSubtr = "mbbSubtr_"+names[0];

  TH1D * hist = new TH1D(nameHist,"",Nbins,xmin,xmax);
  TH1D * histSubtr = new TH1D(nameHistSubtr,"",NbinsBkg,xmin,xmax); 

  TCanvas * dummy = new TCanvas("dummy","",800,700);

  tree->Draw("mbb_reg>>"+nameHist,cuts[0]);
  tree->Draw("mbb_reg>>"+nameHistSubtr,cuts[0]);  

  for (auto sample_name : sample_names) {
    TNtuple * tuple = (TNtuple*)file->Get("Mass_and_BDT_"+sample_name);
    TH1D * hist_mc = new TH1D(nameHistSubtr+sample_name,"",NbinsBkg,xmin,xmax);
    tuple->Draw("mbb_reg>>"+nameHistSubtr+sample_name,"weight*("+cuts[0]+")");
    histSubtr->Add(hist_mc,-1.0);
  }

  RooRealVar mbb("mbb_"+names[0],"mass(bb)",xmin,xmax);

  RooRealVar a0("a0_"+names[0],"a0",-1,1);
  RooRealVar a1("a1_"+names[0],"a1",-1,1);
  RooRealVar a2("a2_"+names[0],"a2",-1,1);
  RooRealVar a3("a3_"+names[0],"a3",-1,1);
  RooRealVar a4("a4_"+names[0],"a4",-1,1);
  RooRealVar a5("a5_"+names[0],"a5",-1,1);
  RooRealVar a6("a6_"+names[0],"a6",-1,1);

  a0.setConstant(false);
  a1.setConstant(false);
  a2.setConstant(false);
  a3.setConstant(false);
  a4.setConstant(false);
  a5.setConstant(false);
  a6.setConstant(false);

  double yield = histSubtr->GetSumOfWeights();
  RooRealVar qcd_yield("qcd_"+names[0]+"_norm","Yield",yield,0.,2*yield);

  RooArgList argList(a0,a1);
  
  if (iORDER>=3)
    argList.add(a2);
  if (iORDER>=4)
    argList.add(a3);
  if (iORDER>=5)
    argList.add(a4);
  if (iORDER>=6)
    argList.add(a5);
  if (iORDER>=7)
    argList.add(a6);
  
  RooChebychev CHV("qcd_"+names[0],"qcd_"+names[0],mbb,argList);
  RooDataHist dataSubtr("dataSubtr_"+names[0],"data",mbb,histSubtr);
  RooDataHist data("data_"+names[0],"data",mbb,hist);

  RooFitResult * res = CHV.fitTo(dataSubtr,Save(),SumW2Error(kTRUE));  
  res->Print();
  RooChi2Var chi2Roo = RooChi2Var("chi2_"+names[0],"chi2",CHV,dataSubtr) ;
  double chi2 = chi2Roo.getVal();
  double ndof = NbinsBkg - iORDER;
  double prob = TMath::Prob(chi2,ndof);
  outtext << endl;
  outtext << "++++++++ " << names[0] << " ++++++++++++++" << endl;
  outtext << endl;
  outtext << "Chi2/ndof = " << chi2 << "/" << ndof << " = " << chi2/ndof << "   prob = " << prob << endl;
  outtext << "TOTAL DATA YIELD = " << hist->GetSumOfWeights() << std::endl;
  outtext << "TOTAL QCD YIELD  = " << yield << endl;
  outtext << endl;

  w->import(CHV);
  w->import(data);
  w->import(qcd_yield);

  for (int iCAT=1; iCAT<5; ++iCAT) {

    TString nameHist_ = "mbb_"+names[iCAT];
    TString nameHistSubtr_ = "mbbSubtr_"+names[iCAT];
    TH1D * hist_ = new TH1D(nameHist_,"",Nbins,xmin,xmax);

    TH1D * histSubtr_ = new TH1D(nameHistSubtr_,"",NbinsBkg,xmin,xmax); // coarser binning
    tree->Draw("mbb_reg>>"+nameHist_,cuts[iCAT]);    
    tree->Draw("mbb_reg>>"+nameHistSubtr_,cuts[iCAT]);    
    
    for (auto sample_name : sample_names) {
      TNtuple * tuple_ = (TNtuple*)file->Get("Mass_and_BDT_"+sample_name);
      TH1D * hist_mc_ = new TH1D(nameHistSubtr_+sample_name,"",NbinsBkg,xmin,xmax);
      tuple_->Draw("mbb_reg>>"+nameHistSubtr_+sample_name,"weight*("+cuts[iCAT]+")");
      histSubtr_->Add(hist_mc_,-1.0);
    }

    double yield_ = histSubtr_->GetSumOfWeights();
    
    TH1D * ratio = DivideHist(histSubtr_,histSubtr,"ratio_"+names[iCAT]);

    RooRealVar mbb_("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

    RooRealVar p1("p1_"+names[iCAT],"p1",-10.,10.);
    RooRealVar p2("p2_"+names[iCAT],"p2",-10.,10.);
    RooRealVar p3("p3_"+names[iCAT],"p3",-10.,10.);
    RooRealVar p4("p4_"+names[iCAT],"p4",-10.,10.);

    p1.setConstant(false);
    p2.setConstant(false);
    p3.setConstant(false);
    p4.setConstant(false);

    RooArgList argListTF(p1);

    if (porder[iCAT]>=2) {
      argListTF.add(p2);
    }
    if (porder[iCAT]>=3) {
      argListTF.add(p3);
    }
    if (porder[iCAT]>=4) {
      argListTF.add(p4);
    }

    std::string polynomName = "tf_" + namesCAT.at(iCAT);
    RooGenericPdf tf(polynomName.c_str(),genericPolynoms.at(porder[iCAT]).c_str(),RooArgSet(mbb_,argListTF));
    RooDataHist dataRatio("data_ratio_"+names[iCAT],"data",mbb_,ratio);
    RooDataHist data_("data_"+names[iCAT],"data",mbb_,hist_);
    RooFitResult * fitRes = tf.fitTo(dataRatio,Save(),SumW2Error(kTRUE));  
    outtext << endl;
    outtext << "++++++++ " << names[iCAT] << " ++++++++++++++" << endl;
    fitRes->Print();
    RooChi2Var chi2Roo_ = RooChi2Var("chi2"+names[iCAT],"chi2",tf,dataRatio) ;
    double chi2_ = chi2Roo_.getVal();
    double ndof_ = NbinsBkg - porder[iCAT];
    double prob_ = TMath::Prob(chi2_,ndof_);
    outtext << endl;
    outtext << "Chi2/ndof = " << chi2_ << "/" << ndof_ << " = " << chi2_/ndof_ << "   prob = " << prob_ << endl;
    outtext << "TOTAL DATA YIELD = " << hist_->GetSumOfWeights() << std::endl;
    outtext << "TOTAL QCD YIELD  = " << yield_ << endl;
    outtext << endl;
    RooChebychev baseFunc("qcd_aux_"+names[iCAT],"qcd_aux"+names[iCAT],mbb_,argList);
    RooProdPdf qcd("qcd_"+names[iCAT],"qcd_"+names[iCAT],RooArgSet(baseFunc,tf));
    double yield = histSubtr_->GetSumOfWeights();
    RooRealVar qcd_yield_("qcd_"+names[iCAT]+"_norm","Yield",yield_,0,2*yield_);

    w->import(qcd);
    w->import(data_);
    w->import(qcd_yield_);
  }

  delete dummy;

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
