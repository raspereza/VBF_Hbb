#include "Common.h"
// TF = true  : background model with the transfer functions
// TF = false : background model based on Bernstein polynomials, QCD spectra are fit independently in different categories 
void CreateDatacards_all(bool TF = true) {

  using namespace RooFit;

  TString suffix("");
  if (TF)
    suffix = "_TF";

  TFile * file = new TFile("root_shape/data_singleb_nominalModel_TF_14_10_2020.root");
  TFile * fileSig = new TFile("root_shape/signal_SingleB_14_11_2020.root");
  TFile * fileTop = new TFile("root_shape/tt_singleb_shapes_14_11_2020.root");
  TFile * fileZj  = new TFile("root_shape/zjets_singleb_shapes_14_11_2020.root");


  RooWorkspace * w = (RooWorkspace*)file->Get("w");
  RooWorkspace * wSig = (RooWorkspace*)fileSig->Get("w");
  RooWorkspace * wtt = (RooWorkspace*)fileTop->Get("w");  
  RooWorkspace * wzj = (RooWorkspace*)fileZj->Get("w");


  for (int i=0; i<5; ++i) {

    RooRealVar * qqHRateVar = wSig->var("vbf_yield_"+names[i]);
    RooRealVar * ggHRateVar = wSig->var("ggh_yield_"+names[i]);
    RooRealVar * qcdRateVar = w->var("qcd_yield_"+names[i]);
    RooRealVar * ttRateVar = wtt->var("tt_yield_"+names[i]);
    RooRealVar * wjRateVar = wzj->var("zj_yield_"+names[i]);
    
    float qqH_rate = qqHRateVar->getValV();
    float ggH_rate = ggHRateVar->getValV();
    float qcd_rate = qcdRateVar->getValV();
    float tt_rate  = ttRateVar->getValV();
    float wj_rate  = wjRateVar->getValV();

    RooRealVar * meanVar = wSig->var("mean_sig_"+names[i]);
    RooRealVar * sigmaVar = wSig->var("sigma_sig_"+names[i]);

    float mean = meanVar->getValV();
    float meanE = meanVar->getError();

    float sigma = sigmaVar->getValV();
    float sigmaE = sigmaVar->getError();

    ostringstream str;
    str << "datacard/datacards_singleb_" << names[i] << "_NominalModel" << suffix << ".txt";
    string nn = str.str();
    const char * p = nn.c_str();
    std::ofstream textfile(p);
    textfile << "imax 1" << endl;
    textfile << "jmax *" << endl;
    textfile << "kmax *" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "shapes          qcd             *   root_shape/data_singleb_nominalModel_TF_14_10_2020.root       w:qcd_$CHANNEL" << endl;
    textfile << "shapes          data_obs        *   root_shape/data_singleb_nominalModel_TF_14_10_2020.root       w:data_$CHANNEL" << endl;
    textfile << "shapes          qqH_hbb         *   root_shape/signal_SingleB_14_11_2020.root   w:sig_$CHANNEL" << endl;
    textfile << "shapes          ggH_hbb         *   root_shape/signal_SingleB_14_11_2020.root   w:sig_$CHANNEL" << endl;
    textfile << "shapes          tt              *   root_shape/tt_singleb_shapes_14_11_2020.root  w:tt_$CHANNEL" << endl;
    textfile << "shapes          zj              *   root_shape/zjets_singleb_shapes_14_11_2020.root  w:zj_$CHANNEL" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << endl;
    textfile << "observation   -1" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << "   " << names[i] << "   " << names[i] << "   " <<   names[i] << "   " << names[i] <<  endl;
    textfile << "process          ggH_hbb   qqH_hbb   qcd   tt    zj " << endl;
    textfile << "process            -1         0       1     2    3 " << endl;
    textfile << "rate            " << ggH_rate << "  " << qqH_rate << " " << qcd_rate <<  " " << tt_rate << "  " << wj_rate << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_ggH_sys       lnN  1.15   -      -     -    -" << endl;
    textfile << "CMS_qqH_sys       lnN   -    1.15    -     -    -" << endl;
    textfile << "CMS_sig_corrsys   lnN  1.20  1.10    -     -    -" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_vbfbb_scale_mbb_13TeV_2018  param 1.0 0.015" << endl; 
    textfile << "CMS_vbfbb_res_mbb_13TeV_2018    param 1.0 0.035" << endl;
    //    textfile << "mean_sig_" << names[i] << "     param " << mean << " " << meanE << endl;
    //    textfile << "sigma_sig_" << names[i] << "    param " << sigma << " " << sigmaE << endl;
    //    textfile << "bkgNorm_" << names[i] << "  rateParam  " << names[i] << "   qcd   1   [0.5,1.5]" << endl;
    textfile << endl;
  }

}
