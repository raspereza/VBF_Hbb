#include "Common.h"

using namespace RooFit;

std::ofstream outtext("wj_out_full.txt");

void CreateWPDF(int iCAT,
		 TNtuple * tree,
		 RooWorkspace * w) {

  TH1::SetDefaultSumw2(true);

  TString nameHist = "mbb_wj"+names[iCAT];
  TH1D * hist = new TH1D(nameHist,"",NbinsSig,xmin,xmax);

  TCanvas * dummy = new TCanvas("dummy","",800,700);
  tree->Draw("mbb_reg>>"+nameHist,"weight*("+cuts[iCAT]+")");
  
  Float_t yieldwj = hist->GetSumOfWeights();
  RooRealVar yield_wj("wj_"+names[iCAT]+"_norm","Yield",yieldwj,0.,2*yieldwj);

  delete hist;
  hist = new TH1D(nameHist,"",Nbins_wj,xmin_wj,xmax_wj);
  tree->Draw("mbb_reg>>"+nameHist,"weight");
  
  delete dummy;

  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);
  RooRealVar mbb_wj("mbb_wj_"+names[iCAT],"mass(bb)",xmin_wj,xmax_wj);

  RooRealVar b0("b0_wj_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_wj_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_wj_"+names[iCAT],"b2",0,1);

  RooBernstein BRNx("brnx","Bernstein",mbb_wj,RooArgList(b0,b1,b2));

  RooDataHist data("data","data",mbb_wj,hist);
  RooFitResult * res = BRNx.fitTo(data,Save(),SumW2Error(kTRUE));

  b0.setConstant(true);
  b1.setConstant(true);
  b2.setConstant(true);

  RooBernstein wj("wj_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));

  outtext << endl;
  outtext << "+++++++++++++ " << names[iCAT] << "++++++++++ " << endl;
  std::cout << endl;
  b0.Print();
  b1.Print();
  b2.Print();
  outtext  << "yields : wj = " << yieldwj << std::endl; 
  res->Print();

  w->import(wj);
  w->import(yield_wj);

}
void  CreateWJetsTemplates() {

  TFile * file = new TFile("ntuples/mbb_and_bdt_all_Nom.root");
  TNtuple * tree = (TNtuple*)file->Get("Mass_and_BDT_WJets");

  TFile * fileOutput = new TFile("root_shape/wj_singleb_shapes_2018.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","w");

  for (int i=1; i<4; ++i) 
    CreateWPDF(i,tree,w);

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
