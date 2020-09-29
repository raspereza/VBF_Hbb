#include "Common.h"

using namespace RooFit;

void CreatePDF (int iCAT,
		int iORDER,
		TNtuple * tree,
		RooWorkspace * w) {

  TH1::SetDefaultSumw2(true);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  TString nameHist = "mbb_"+names[iCAT];
  TH1D * hist = new TH1D(nameHist,"",Nbins,xmin,xmax);
  tree->Draw("mbb>>"+nameHist,cuts[iCAT]);
  delete dummy;

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);
  RooRealVar b0("b0_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_"+names[iCAT],"b2",0,1);
  RooRealVar b3("b3_"+names[iCAT],"b3",0,1);
  RooRealVar b4("b4_"+names[iCAT],"b4",0,1);
  RooRealVar b5("b5_"+names[iCAT],"b5",0,1);
  b0.setConstant(false);
  b1.setConstant(false);
  b2.setConstant(false);
  b3.setConstant(false);
  b4.setConstant(false);
  b5.setConstant(false);

  Float_t yield = hist->GetSumOfWeights();
  RooRealVar qcd_yield("qcd_yield_"+names[iCAT],"Yield",yield,0.5*yield,2*yield);

  RooArgList argList(b0,b1,b2);
  if (iORDER>=4)
    argList.add(b3);
  if (iORDER>=5)
    argList.add(b4);
  if (iORDER>=6)
    argList.add(b5);
  
  RooBernstein BRN("qcd_"+names[iCAT],"qcd_"+names[iCAT],mbb,argList);
  RooDataHist data("data_"+names[iCAT],"data",mbb,hist);

  RooFitResult * res = BRN.fitTo(data,Save());  

  cout << endl;
  cout << "+++++++++++++++ " << names[iCAT] << " +++++++++++++++++++" << endl;
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

}

void CreateDataTemplates() {

  TFile * file = new TFile("mbb_and_BDT.root");
  TNtuple * tree = (TNtuple*)file->Get("MinvO");

  TFile * fileOutput = new TFile("data_singleb_shapes.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","data");

  int iorder[5] = {6,3,3,3,3};

  for (int i=0; i<5; ++i) 
    CreatePDF(i,iorder[i],tree,w);
  
  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
