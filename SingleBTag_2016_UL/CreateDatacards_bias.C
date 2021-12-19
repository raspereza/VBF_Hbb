#include "Common.h"
// TF = true  : background model with the transfer functions
// TF = false : background model based on Bernstein polynomials, QCD spectra are fit independently in different categories 
double hard_sysval[] = {1.14,1.02,1.20,1.16,1.45};
void CreateDatacards_bias(bool TF = false,
			  bool freeZnorm = false) {

  using namespace RooFit;

  TString suffix("");
  if (TF)
    suffix = "_CHV_4_TF_Ord_2";

  TFile * file = new TFile("root_shape/data_singleb_shapes"+suffix+".root");
  TFile * fileSig = new TFile("root_shape/signal_singleb_shapes.root");
  TFile * fileTop = new TFile("root_shape/tt_singleb_shapes.root");
  TFile * fileZj  = new TFile("root_shape/zj_singleb_shapes.root");
  TFile * fileWj  = new TFile("root_shape/wj_singleb_shapes.root");


  RooWorkspace * w = (RooWorkspace*)file->Get("w");
  RooWorkspace * wSig = (RooWorkspace*)fileSig->Get("w");
  RooWorkspace * wtt = (RooWorkspace*)fileTop->Get("w");  
  RooWorkspace * wzj = (RooWorkspace*)fileZj->Get("w");
  RooWorkspace * wwj = (RooWorkspace*)fileWj->Get("w");


  TFile  *fs = new TFile("VBFHToBB_singleB_2016_UL_10_11_2021.root");

  TH1F * vbfh_btg_up = (TH1F*)fs->Get("VBF_Hbb_BTG_up");
  TH1F * vbfh_btg_dn = (TH1F*)fs->Get("VBF_Hbb_BTG_dn");
  TH1F * vbfh_pu_up = (TH1F*)fs->Get("VBF_Hbb_PU_up");
  TH1F * vbfh_pu_dn = (TH1F*)fs->Get("VBF_Hbb_PU_dn");
  TH1F * vbfh_isr_up = (TH1F*)fs->Get("VBF_Hbb_ISR_up");
  TH1F * vbfh_isr_dn = (TH1F*)fs->Get("VBF_Hbb_ISR_dn");
  TH1F * vbfh_fsr_up = (TH1F*)fs->Get("VBF_Hbb_FSR_up");
  TH1F * vbfh_fsr_dn = (TH1F*)fs->Get("VBF_Hbb_FSR_dn");
  TH1F * vbfh_puid_up = (TH1F*)fs->Get("VBF_Hbb_PUID_up");
  TH1F * vbfh_puid_dn = (TH1F*)fs->Get("VBF_Hbb_PUID_dn");
  TH1F * vbfh_trig_up = (TH1F*)fs->Get("VBF_Hbb_TRIG_up");
  TH1F * vbfh_trig_dn = (TH1F*)fs->Get("VBF_Hbb_TRIG_dn");
  TH1F * vbfh_qgl = (TH1F*)fs->Get("VBF_Hbb_QGL_up");

  TH1F * ggh_btg_up = (TH1F*)fs->Get("ggF_Hbb_BTG_up");
  TH1F * ggh_btg_dn = (TH1F*)fs->Get("ggF_Hbb_BTG_dn");
  TH1F * ggh_pu_up = (TH1F*)fs->Get("ggF_Hbb_PU_up");
  TH1F * ggh_pu_dn = (TH1F*)fs->Get("ggF_Hbb_PU_dn");
  TH1F * ggh_isr_up = (TH1F*)fs->Get("ggF_Hbb_ISR_up");
  TH1F * ggh_isr_dn = (TH1F*)fs->Get("ggF_Hbb_ISR_dn");
  TH1F * ggh_fsr_up = (TH1F*)fs->Get("ggF_Hbb_FSR_up");
  TH1F * ggh_fsr_dn = (TH1F*)fs->Get("ggF_Hbb_FSR_dn");
  TH1F * ggh_puid_up = (TH1F*)fs->Get("ggF_Hbb_PUID_up");
  TH1F * ggh_puid_dn = (TH1F*)fs->Get("ggF_Hbb_PUID_dn");
  TH1F * ggh_trig_up = (TH1F*)fs->Get("ggF_Hbb_TRIG_up");
  TH1F * ggh_trig_dn = (TH1F*)fs->Get("ggF_Hbb_TRIG_dn");
  TH1F * ggh_qgl = (TH1F*)fs->Get("ggF_Hbb_QGL_up");


  TH1F * tt_btg_up = (TH1F*)fs->Get("tt_BTG_up");
  TH1F * tt_btg_dn = (TH1F*)fs->Get("tt_BTG_dn");
  TH1F * tt_pu_up = (TH1F*)fs->Get("tt_PU_up");
  TH1F * tt_pu_dn = (TH1F*)fs->Get("tt_PU_dn");
  TH1F * tt_isr_up = (TH1F*)fs->Get("tt_ISR_up");
  TH1F * tt_isr_dn = (TH1F*)fs->Get("tt_ISR_dn");
  TH1F * tt_fsr_up = (TH1F*)fs->Get("tt_FSR_up");
  TH1F * tt_fsr_dn = (TH1F*)fs->Get("tt_FSR_dn");
  TH1F * tt_puid_up = (TH1F*)fs->Get("tt_PUID_up");
  TH1F * tt_puid_dn = (TH1F*)fs->Get("tt_PUID_dn");
  TH1F * tt_trig_up = (TH1F*)fs->Get("tt_TRIG_up");
  TH1F * tt_trig_dn = (TH1F*)fs->Get("tt_TRIG_dn");
  TH1F * tt_qgl = (TH1F*)fs->Get("tt_QGL_up");

  TH1F * wj_btg_up = (TH1F*)fs->Get("WJets_BTG_up");
  TH1F * wj_btg_dn = (TH1F*)fs->Get("WJets_BTG_dn");
  TH1F * wj_pu_up = (TH1F*)fs->Get("WJets_PU_up");
  TH1F * wj_pu_dn = (TH1F*)fs->Get("WJets_PU_dn");
  TH1F * wj_isr_up = (TH1F*)fs->Get("WJets_ISR_up");
  TH1F * wj_isr_dn = (TH1F*)fs->Get("WJets_ISR_dn");
  TH1F * wj_fsr_up = (TH1F*)fs->Get("WJets_FSR_up");
  TH1F * wj_fsr_dn = (TH1F*)fs->Get("WJets_FSR_dn");
  TH1F * wj_puid_up = (TH1F*)fs->Get("WJets_PUID_up");
  TH1F * wj_puid_dn = (TH1F*)fs->Get("WJets_PUID_dn");
  TH1F * wj_trig_up = (TH1F*)fs->Get("WJets_TRIG_up");
  TH1F * wj_trig_dn = (TH1F*)fs->Get("WJets_TRIG_dn");
  TH1F * wj_qgl = (TH1F*)fs->Get("WJets_QGL_up");

  TH1F * zj_btg_up = (TH1F*)fs->Get("ZJets_BTG_up");
  TH1F * zj_btg_dn = (TH1F*)fs->Get("ZJets_BTG_dn");
  TH1F * zj_pu_up = (TH1F*)fs->Get("ZJets_PU_up");
  TH1F * zj_pu_dn = (TH1F*)fs->Get("ZJets_PU_dn");
  TH1F * zj_isr_up = (TH1F*)fs->Get("ZJets_ISR_up");
  TH1F * zj_isr_dn = (TH1F*)fs->Get("ZJets_ISR_dn");
  TH1F * zj_fsr_up = (TH1F*)fs->Get("ZJets_FSR_up");
  TH1F * zj_fsr_dn = (TH1F*)fs->Get("ZJets_FSR_dn");
  TH1F * zj_puid_up = (TH1F*)fs->Get("ZJets_PUID_up");
  TH1F * zj_puid_dn = (TH1F*)fs->Get("ZJets_PUID_dn");
  TH1F * zj_trig_up = (TH1F*)fs->Get("ZJets_TRIG_up");
  TH1F * zj_trig_dn = (TH1F*)fs->Get("ZJets_TRIG_dn");
  TH1F * zj_qgl = (TH1F*)fs->Get("ZJets_QGL_up");

  TH1F * vbfh_diploe = (TH1F*)fs->Get("VBF_Hbb_Dipole_RecoilOn_up");
  for (int i=0; i<5; ++i) {

    //    RooRealVar * meanVar = wSig->var("mean_sig_"+names[i]);
    //    RooRealVar * sigmaVar = wSig->var("sigma_sig_"+names[i]);

    //    float mean = meanVar->getValV();
    //    float meanE = meanVar->getError();

    //    float sigma = sigmaVar->getValV();
    //    float sigmaE = sigmaVar->getError();

    RooRealVar * rate_ggH_var = wSig->var("norm_GGH_Nom_sig_"+names[i]);
    RooRealVar * rate_qqH_var = wSig->var("norm_VBF_Nom_sig_"+names[i]);
    RooRealVar * rate_tt_var = wtt->var("norm_Nom_tt_"+names[i]);
    RooRealVar * rate_wj_var = wwj->var("norm_Nom_wj_"+names[i]);
    RooRealVar * rate_zj_var = wzj->var("norm_Nom_zj_"+names[i]);


    double ggH_rate = rate_ggH_var->getValV();
    double qqH_rate = rate_qqH_var->getValV();
    double tt_rate = rate_tt_var->getValV();
    double wj_rate = rate_wj_var->getValV();
    double zj_rate = rate_zj_var->getValV();

    ostringstream str;
    str << "datacards/datacards_singleb_" << names[i] << "_2016.txt";
    string nn = str.str();
    const char * p = nn.c_str();
    std::ofstream textfile(p);
    textfile << "imax 1" << endl;
    textfile << "jmax *" << endl;
    textfile << "kmax *" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "shapes          qcd             *   SingleBTag_2016_UL/root_shape/data_singleb_shapes.root       w:qcd_$CHANNEL" << endl;
    textfile << "shapes          data_obs        *   SingleBTag_2016_UL/root_shape/data_singleb_shapes.root       w:data_$CHANNEL" << endl;
    textfile << "shapes          qqH_hbb         *   SingleBTag_2016_UL/root_shape/signal_singleb_shapes.root   w:qqH_$CHANNEL" << endl;
    textfile << "shapes          ggH_hbb         *   SingleBTag_2016_UL/root_shape/signal_singleb_shapes.root   w:ggH_$CHANNEL" << endl;
    textfile << "shapes          qqH_hbb_bias    *   SingleBTag_2016_UL/root_shape/signal_singleb_shapes.root   w:qqH_$CHANNEL" << endl;
    textfile << "shapes          ggH_hbb_bias    *   SingleBTag_2016_UL/root_shape/signal_singleb_shapes.root   w:ggH_$CHANNEL" << endl;
    textfile << "shapes          tt              *   SingleBTag_2016_UL/root_shape/tt_singleb_shapes.root  w:tt_$CHANNEL" << endl;
    textfile << "shapes          wj              *   SingleBTag_2016_UL/root_shape/wj_singleb_shapes.root  w:wj_$CHANNEL" << endl;
    textfile << "shapes          zj              *   SingleBTag_2016_UL/root_shape/zj_singleb_shapes.root  w:zj_$CHANNEL" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << endl;
    textfile << "observation   -1" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << "   " << names[i] << "   " << names[i] << "   " <<   names[i] << "   " << names[i] << "   " << names[i] << "   " << names[i] << "    " << names[i] << endl;
    textfile << "process          ggH_hbb   qqH_hbb   qcd   tt    wj    zj   ggH_hbb_bias  qqH_hbb_bias" << endl;
    textfile << "process            -1         0       1     2    3     4        5              6" << endl;
    textfile << "rate     1     1    1    1    1    1    1    1 " << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_BR_hbb     lnN	1.007/0.994	1.007/0.994	-      -     -     -     1.007/0.994     1.007/0.994" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;

    textfile << "lumi_13TeV_BCC_2016  lnN  1.002    1.002    -     1.002   1.002   1.002   1.002   1.002" << endl;
    textfile << "lumi_13TeV_LS_2016   lnN  1.002    1.002    -    1.002   1.002   1.002   1.002    1.002" << endl;
    textfile << "lumi_13TeV_XY_2016   lnN  1.020    1.020    -    1.020   1.020   1.020   1.020    1.020" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    //Offline BTAG
    textfile << fixed << setprecision(3)<< "CMS_BTag_2016       lnN     " << (1 + ggh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_btg_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_btg_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_btg_up->GetBinContent(i+1)) << "/" <<  (1 + tt_btg_dn->GetBinContent(i+1)) << "      " << (1 + wj_btg_up->GetBinContent(i+1)) << "/" <<  (1 + wj_btg_dn->GetBinContent(i+1)) << "   " << (1 + zj_btg_up->GetBinContent(i+1)) << "/" <<  (1 + zj_btg_dn->GetBinContent(i+1)) << "   " << (1 + ggh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_btg_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_btg_dn->GetBinContent(i+1))   << endl;
    //PU - reweighting
    textfile << fixed << setprecision(3)<< "CMS_PUweight_2016   lnN     " << (1 + ggh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_pu_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_pu_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_pu_up->GetBinContent(i+1)) << "/" <<  (1 + tt_pu_dn->GetBinContent(i+1)) << "      " << (1 + wj_pu_up->GetBinContent(i+1)) << "/" <<  (1 + wj_pu_dn->GetBinContent(i+1)) << "   " << (1 + zj_pu_up->GetBinContent(i+1)) << "/" <<  (1 + zj_pu_dn->GetBinContent(i+1)) << "   "  <<  (1 + ggh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_pu_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_pu_dn->GetBinContent(i+1))  << endl;
    //QGL - reweighting
    textfile << fixed << setprecision(3)<< "CMS_QGLweight_2016  lnN     " << (1 + ggh_qgl->GetBinContent(i+1)) <<  "  " << (1 + vbfh_qgl->GetBinContent(i+1)) << "        -       " <<   (1 + tt_qgl->GetBinContent(i+1)) << "      " << (1 + zj_qgl->GetBinContent(i+1))  << "   " << (1 + zj_qgl->GetBinContent(i+1))  << "   "  << (1 + ggh_qgl->GetBinContent(i+1)) <<  "  " << (1 + vbfh_qgl->GetBinContent(i+1)) << endl;
    //ISR
    textfile << fixed << setprecision(3)<< "CMS_PS_ISR_2016     lnN     " << (1 + ggh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_isr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_isr_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_isr_up->GetBinContent(i+1)) << "/" <<  (1 + tt_isr_dn->GetBinContent(i+1)) << "      " << (1 + wj_isr_up->GetBinContent(i+1)) << "/" <<  (1 + wj_isr_dn->GetBinContent(i+1))  << "   " << "      " << (1 + zj_isr_up->GetBinContent(i+1)) << "/" <<  (1 + zj_isr_dn->GetBinContent(i+1))  << "   " << (1 + ggh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_isr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_isr_dn->GetBinContent(i+1)) << endl;
    //FSR
    textfile << fixed << setprecision(3)<< "CMS_PS_FSR_2016     lnN     " << (1 + ggh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_fsr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_fsr_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + tt_fsr_dn->GetBinContent(i+1)) << "      " << (1 + wj_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + wj_fsr_dn->GetBinContent(i+1)) << "   " << (1 + zj_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + zj_fsr_dn->GetBinContent(i+1)) << "   " <<  (1 + ggh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_fsr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_fsr_dn->GetBinContent(i+1))  << endl;
    //Online TRIG
    textfile << fixed << setprecision(3)<< "CMS_Trig_2016     lnN     " << (1 + ggh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_trig_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_trig_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_trig_up->GetBinContent(i+1)) << "/" <<  (1 + tt_trig_dn->GetBinContent(i+1)) << "      " << (1 + wj_trig_up->GetBinContent(i+1)) << "/" <<  (1 + wj_trig_dn->GetBinContent(i+1)) << "   " << (1 + zj_trig_up->GetBinContent(i+1)) << "/" <<  (1 + zj_trig_dn->GetBinContent(i+1)) << "   " << (1 + ggh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_trig_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_trig_dn->GetBinContent(i+1))  << endl;
    //PUID
    textfile << fixed << setprecision(3)<< "CMS_PileUpID_2016     lnN     " << (1 + ggh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_puid_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_puid_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_puid_up->GetBinContent(i+1)) << "/" <<  (1 + tt_puid_dn->GetBinContent(i+1)) << "      " << (1 + wj_puid_up->GetBinContent(i+1)) << "/" <<  (1 + wj_puid_dn->GetBinContent(i+1)) << "   " << (1 + zj_puid_up->GetBinContent(i+1)) << "/" <<  (1 + zj_puid_dn->GetBinContent(i+1)) << "   " << (1 + ggh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_puid_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_puid_dn->GetBinContent(i+1))  << endl;
    //Dipole
    textfile << fixed << setprecision(3)<< "VBF_Dipole_Recoil	lnN	" << "	-	" << (1 + vbfh_diploe->GetBinContent(i+1)) << "	-	"	<< "	-	" <<	"	-	"	<<	"	-	"	<<	"	-	"	<< 	(1 + vbfh_diploe->GetBinContent(i+1)) << endl; 
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_norm_wj       lnN   -     -      -     -  1.3  -  -  -" << endl;
    if (!freeZnorm) textfile << "CMS_norm_zj       lnN   -     -      -     -   -  1.3  -  -" << endl;
    textfile << "CMS_norm_tt       lnN   -     -      -   1.3   -    -  -  -" << endl;   
    textfile << "QCDscale_ggH      lnN  1.046/0.933   -    -    -    -  -  1.046/0.933   -" << endl;
    textfile << "PDFscale_ggH      lnN  1.019/0.981   -    -    -    -  -  1.019/0.981   -" << endl;
    textfile << "alphas_ggH        lnN  1.026/0.974   -    -    -    -  -  1.026/0.974   -" << endl;
    textfile << "QCDscale_qqH      lnN  -   1.004/0.997    -    -    -  -       -   1.004/0.997" << endl;
    textfile << "PDFscale_qqH      lnN  -   1.021/0.979    -    -    -  -       -   1.021/0.979" << endl;
    textfile << "alphas_qqH        lnN  -   1.005/0.995    -    -    -  -       -   1.005/0.995" << endl;
    textfile << "CMS_PDF_set       lnN    1.020   1.016    -    -    -  -     1.020  1.016" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_JES_2016  param 0.0 1.0" << endl;
    textfile << "CMS_JER_2016  param 0.0 1.0" << endl;
    textfile << "bias_" << names[i] << " rateParam " << names[i] << " ggH_hbb_bias  0 [-1,1] " << endl; 
    textfile << "bias_" << names[i] << " rateParam " << names[i] << " qqH_hbb_bias  0 [-1,1] " << endl; 
    textfile << "bias_" << names[i] << " param 0 0.2" << endl;
    if (freeZnorm) textfile << "normZ " << " rateParam " << names[i] << " zj 1 [-100,100]" << endl;
    textfile << endl;
  }

}
