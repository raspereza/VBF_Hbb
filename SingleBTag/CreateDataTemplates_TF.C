#include "Common.h"

using namespace RooFit;

void CreateDataTemplates_TF() {

  int iORDER = 6; // Order of BRN polynome for baseline function 
  int iorder[5] = {1,2,2,2,2}; // order polynomials of the transfer functions

  TFile * file = new TFile("/afs/cern.ch/work/m/mukherje/public/ForVBFHbb/mbb_and_bdt_all_BJETbtg.root");
  TTree * tree = (TTree*)file->Get("Mass_and_BDT_DATA");

  TFile * fileOutput = new TFile("root_shape/data_singleb_nominalModel_TF_14_10_2020.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","data");
  
  TH1::SetDefaultSumw2(true);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_"+names[0];
  TString nameHistRatio = "mbb_ratio_"+names[0];
  TH1D * hist = new TH1D(nameHist,"",Nbins,xmin,xmax);
  TH1D * histRatio = new TH1D(nameHistRatio,"",NbinsBkg,xmin,xmax); 
  tree->Draw("mbb>>"+nameHist,cuts[0]);
  tree->Draw("mbb>>"+nameHistRatio,cuts[0]);
  delete dummy;

  RooRealVar mbb("mbb_"+names[0],"mass(bb)",xmin,xmax);
  RooRealVar b0("b0_"+names[0],"b0",0,1);
  RooRealVar b1("b1_"+names[0],"b1",0,1);
  RooRealVar b2("b2_"+names[0],"b2",0,1);
  RooRealVar b3("b3_"+names[0],"b3",0,1);
  RooRealVar b4("b4_"+names[0],"b4",0,1);
  RooRealVar b5("b5_"+names[0],"b5",0,1);
  b0.setConstant(false);
  b1.setConstant(false);
  b2.setConstant(false);
  b3.setConstant(false);
  b4.setConstant(false);
  b5.setConstant(false);

  Float_t yield = hist->GetSumOfWeights();
  RooRealVar qcd_yield("qcd_yield_"+names[0],"Yield",yield,0.5*yield,2*yield);

  RooArgList argList(b0,b1,b2);
  if (iORDER>=4)
    argList.add(b3);
  if (iORDER>=5)
    argList.add(b4);
  if (iORDER>=6)
    argList.add(b5);
  
  RooBernstein BRN("qcd_"+names[0],"qcd_"+names[0],mbb,argList);
  RooDataHist data("data_"+names[0],"data",mbb,hist);

  RooFitResult * res = BRN.fitTo(data,Save());  

  cout << endl;
  cout << "+++++++++++++++ " << names[0] << " +++++++++++++++++++" << endl;
  cout << endl;
  b0.Print();
  b1.Print();
  b2.Print();
  b3.Print();
  b4.Print();
  b5.Print();
  res->Print();
  cout << "TOTAL YIELD = " << yield << endl;
  cout << endl;
  cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;
  cout << endl;

  w->import(BRN);
  w->import(data);
  w->import(qcd_yield);

  for (int iCAT=1; iCAT<5; ++iCAT) {

    TCanvas * dummy = new TCanvas("dummy","",800,700);
    TString nameHist_ = "mbb_"+names[iCAT];
    TString nameHistRatio_ = "mbb_ratio_"+names[iCAT];
    TH1D * hist_ = new TH1D(nameHist_,"",Nbins,xmin,xmax);
    TH1D * histRatio_ = new TH1D(nameHistRatio_,"",NbinsBkg,xmin,xmax); // coarser binning
    tree->Draw("mbb>>"+nameHist_,cuts[iCAT]);    
    tree->Draw("mbb>>"+nameHistRatio_,cuts[iCAT]);    
    delete dummy;

    TH1D * ratio = DivideHist(histRatio_,histRatio,"ratio_"+names[iCAT]);

    RooRealVar mbb_("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);
    RooRealVar a1("a1_"+names[iCAT],"a1",-2.,2.);
    RooRealVar a2("a2_"+names[iCAT],"a2",-2.,2.);
    RooRealVar a3("a3_"+names[iCAT],"a3",-2.,2.);
    RooRealVar a4("a4_"+names[iCAT],"a4",-2.,2.);
    a1.setConstant(false);
    a2.setConstant(false);
    a3.setConstant(false);
    a4.setConstant(false);

    RooArgList argListTF(a1);
    if (iorder[iCAT]>=2) {
      argListTF.add(a2);
    }
    if (iorder[iCAT]>=3) {
      argListTF.add(a3);
    }
    if (iorder[iCAT]>=4) {
      argListTF.add(a4);
    }

    std::string polynomName = "tf_" + namesCAT.at(iCAT);
    RooGenericPdf tf(polynomName.c_str(),genericPolynoms.at(iorder[iCAT]).c_str(),RooArgSet(mbb_,argListTF));
    RooDataHist dataRatio("data_ratio_"+names[iCAT],"data",mbb_,ratio);
    RooDataHist data_("data_"+names[iCAT],"data",mbb_,hist_);
    RooFitResult * fitRes = tf.fitTo(dataRatio,Save(),SumW2Error(kTRUE));  
    cout << endl;
    cout << "++++++++ FIT " << names[iCAT] << "++++++++++++++" << endl;
    fitRes->Print();
    cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << endl;

    RooBernstein baseFunc("qcd_aux_"+names[iCAT],"aux",mbb_,argList);
    RooProdPdf qcd("qcd_"+names[iCAT],"qcd"+names[iCAT],RooArgSet(baseFunc,tf));
    Float_t yield = hist_->GetSumOfWeights();
    RooRealVar qcd_yield_("qcd_yield_"+names[iCAT],"Yield",yield,0.5*yield,2*yield);
  
    w->import(qcd);
    w->import(data_);
    w->import(qcd_yield_);
  }

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
