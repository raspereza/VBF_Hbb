#include "Common.h"
// TF = true  : background model with the transfer functions
// TF = false : background model based on Bernstein polynomials, QCD spectra are fit independently in different categories 
void CreateDatacards_sys(bool TF = false) 
{

  using namespace RooFit;

  TFile * file = new TFile("rootshape/data_doubleb_shapes.root");
  TFile * fileSig = new TFile("rootshape/signal_doubleb_shapes.root");
  TFile * fileTop = new TFile("rootshape/tt_doubleb_shapes.root");
  TFile * fileZj  = new TFile("rootshape/zj_doubleb_shapes.root");

  TFile  *fs =  new TFile("Zbb_2018_Sys_Unc_.root");
  RooWorkspace * w = (RooWorkspace*)file->Get("w");
  RooWorkspace * wSig = (RooWorkspace*)fileSig->Get("w");
  RooWorkspace * wtt = (RooWorkspace*)fileTop->Get("w");


  TH1F * vbfh_btg_up = (TH1F*)fs->Get("VBF_Hbb_BTG_up");
  TH1F * vbfh_btg_dn = (TH1F*)fs->Get("VBF_Hbb_BTG_dn");
  TH1F * ggh_btg_up = (TH1F*)fs->Get("ggF_Hbb_BTG_up");
  TH1F * ggh_btg_dn = (TH1F*)fs->Get("ggF_Hbb_BTG_dn");
  TH1F * tt_btg_up = (TH1F*)fs->Get("tt_BTG_up");
  TH1F * tt_btg_dn = (TH1F*)fs->Get("tt_BTG_dn");
  TH1F * zj_btg_up = (TH1F*)fs->Get("ZJets_BTG_up");
  TH1F * zj_btg_dn = (TH1F*)fs->Get("ZJets_BTG_dn");
  TH1F * wj_btg_up = (TH1F*)fs->Get("WJets_BTG_up");
  TH1F * wj_btg_dn = (TH1F*)fs->Get("WJets_BTG_dn");


  for (int i=0; i<4; ++i) 
   {
    ostringstream str;
    str << "datacards/datacards_doubleb_" << names[i] << "_ZBB.txt";
    string nn = str.str();
    const char * p = nn.c_str();
    std::ofstream textfile(p);
    textfile << "imax 1" << endl;
    textfile << "jmax *" << endl;
    textfile << "kmax *" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "shapes          qcd             *   ZBB_Inclusive/rootshape/data_doubleb_shapes.root       w:qcd_$CHANNEL" << endl;
    textfile << "shapes          data_obs        *   ZBB_Inclusive/rootshape/data_doubleb_shapes.root       w:data_$CHANNEL" << endl;
    textfile << "shapes          qqH_hbb         *   ZBB_Inclusive/rootshape/signal_doubleb_shapes.root   w:qqH_$CHANNEL" << endl;
    textfile << "shapes          ggH_hbb         *   ZBB_Inclusive/rootshape/signal_doubleb_shapes.root   w:ggH_$CHANNEL" << endl;
    textfile << "shapes          tt              *   ZBB_Inclusive/rootshape/tt_doubleb_shapes.root  w:tt_$CHANNEL" << endl;
    textfile << "shapes          zj              *   ZBB_Inclusive/rootshape/zj_doubleb_shapes.root  w:zj_$CHANNEL" << endl;
    textfile << "shapes          wj              *   ZBB_Inclusive/rootshape/wj_doubleb_shapes.root  w:wj_$CHANNEL" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << endl;
    textfile << "observation   -1" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << "   " << names[i] << "   " <<   names[i] << "   " << names[i] << "   " << names[i] << "   " << names[i] << endl;
    textfile << "process          zj    qcd   tt   ggH_hbb qqH_hbb   wj" << endl;
    textfile << "process          0      1     2      3      4     5 " << endl;
    textfile << "rate             1      1     1      1      1     1 " << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_BR_hbb     lnN	-	-      -     1.007/0.994     1.007/0.994     -  "   << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "lumi_13TeV_BCC_2018  lnN  1.002       -     1.002   1.002   1.002     -   " << endl;
    textfile << "lumi_13TeV_LS_2018   lnN  1.002       -     1.002   1.002   1.002     -   "  << endl;
    textfile << "lumi_13TeV_XY_2018   lnN  1.020       -     1.020   1.002   1.002     -   "  << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << fixed << setprecision(3)<< "CMS_BTag_2018       lnN     " << (1 + zj_btg_up->GetBinContent(i+1)) << "/" <<  (1 + zj_btg_dn->GetBinContent(i+1)) << "    -     " << (1 + tt_btg_up->GetBinContent(i+1)) << "/" <<  (1 + tt_btg_dn->GetBinContent(i+1)) <<  "  " << (1 + ggh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_btg_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_btg_dn->GetBinContent(i+1)) << "		" << (1 + wj_btg_up->GetBinContent(i+1)) << "/" <<  (1 + wj_btg_dn->GetBinContent(i+1)) << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_norm_tt       lnN   -     -      1.3  -    -    - " << endl;
    textfile << "CMS_norm_tt       lnN   -     -      -    -    -    1.3 " << endl;
    textfile << "QCDscale_ggH      lnN   -     -      -    1.046/0.933   -    - " << endl;
    textfile << "PDFscale_ggH      lnN   -     -      -    1.019/0.981   -    - " << endl;
    textfile << "alphas_ggH        lnN   -     -      -    1.026/0.974   -    - " << endl;
    textfile << "QCDscale_qqH      lnN   -     -      -    -    1.004/0.997    -" << endl;
    textfile << "PDFscale_qqH      lnN   -     -      -    -    1.021/0.979    -" << endl;
    textfile << "alphas_qqH        lnN   -     -      -    -    1.005/0.995    -" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_JES_2018  param 0.0 1.0" << endl;
    textfile << "CMS_JER_2018  param 0.0 1.0" << endl;
    textfile << endl;
   }
 }
