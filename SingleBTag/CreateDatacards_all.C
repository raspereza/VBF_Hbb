#include "Common.h"
// TF = true  : background model with the transfer functions
// TF = false : background model based on Bernstein polynomials, QCD spectra are fit independently in different categories 
void CreateDatacards_all() {

  using namespace RooFit;

  TFile * file = new TFile("root_shape/data_singleb_shapes.root");
  TFile * fileSig = new TFile("root_shape/signal_singleb_shapes.root");
  TFile * fileTop = new TFile("root_shape/tt_singleb_shapes.root");
  TFile * fileZj  = new TFile("root_shape/zjets_singleb_shapes.root");


  RooWorkspace * w = (RooWorkspace*)file->Get("w");
  RooWorkspace * wSig = (RooWorkspace*)fileSig->Get("w");
  RooWorkspace * wtt = (RooWorkspace*)fileTop->Get("w");  
  RooWorkspace * wzj = (RooWorkspace*)fileZj->Get("w");


  for (int i=0; i<5; ++i) {

    ostringstream str;
    str << "datacards_singleb/datacards_singleb_" << names[i] << ".txt";
    string nn = str.str();
    const char * p = nn.c_str();
    std::ofstream textfile(p);
    textfile << "imax 1" << endl;
    textfile << "jmax *" << endl;
    textfile << "kmax *" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "shapes          qcd             *   SingleBTag/root_shape/data_singleb_shapes.root       w:qcd_$CHANNEL" << endl;
    textfile << "shapes          data_obs        *   SingleBTag/root_shape/data_singleb_shapes.root       w:data_$CHANNEL" << endl;
    textfile << "shapes          qqH_hbb         *   SingleBTag/root_shape/signal_singleb_shapes.root   w:qqH_$CHANNEL" << endl;
    textfile << "shapes          ggH_hbb         *   SingleBTag/root_shape/signal_singleb_shapes.root   w:ggH_$CHANNEL" << endl;
    textfile << "shapes          tt              *   SingleBTag/root_shape/tt_singleb_shapes.root  w:tt_$CHANNEL" << endl;
    textfile << "shapes          zj              *   SingleBTag/root_shape/zjets_singleb_shapes.root  w:zj_$CHANNEL" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << endl;
    textfile << "observation   -1" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << "   " << names[i] << "   " << names[i] << "   " <<   names[i] << "   " << names[i] <<  endl;
    textfile << "process          ggH_hbb   qqH_hbb   qcd   tt    zj " << endl;
    textfile << "process            -1         0       1     2    3 " << endl;
    textfile << "rate     1     1    1    1    1" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_ggH_sys       lnN  1.15   -      -     -    -" << endl;
    textfile << "CMS_qqH_sys       lnN   -    1.15    -     -    -" << endl;
    textfile << "CMS_sig_corrsys   lnN  1.20  1.10    -     -    -" << endl;
    textfile << "CMS_norm_zjets    lnN   -     -      -     -  1.3" << endl;
    textfile << "CMS_norm_tt       lnN   -     -      -   1.3    -" << endl;   
    textfile << endl;
  }
}
