#include "Common_sys.h"
// TF = true  : background model with the transfer functions
// TF = false : background model based on Bernstein polynomials, QCD spectra are fit independently in different categories 
void CreateDatacards_bias(bool TF = false) {

  using namespace RooFit;

  TString suffix("");
  if (TF)
    suffix = "_CHV_4_TF_Ord_2";

  TFile * file = new TFile("root_shape/data_doubleb_shapes"+suffix+".root");
  TFile * fileSig = new TFile("root_shape/signal_doubleb_shapes.root");
  TFile * fileTop = new TFile("root_shape/tt_doubleb_shapes.root");
  TFile * fileZj  = new TFile("root_shape/zj_doubleb_shapes.root");

  TFile  *fs = new TFile("VBFHbb_DoubleBTag_Removed_overlap_2018_Sys_Unc.root");

  RooWorkspace * w = (RooWorkspace*)file->Get("w");
  RooWorkspace * wSig = (RooWorkspace*)fileSig->Get("w");
  RooWorkspace * wtt = (RooWorkspace*)fileTop->Get("w");  
  RooWorkspace * wzj = (RooWorkspace*)fileZj->Get("w");

  TH1F * vbfh_jec_up = (TH1F*)fs->Get("VBF_Hbb_JEC_up");
  TH1F * vbfh_jec_dn = (TH1F*)fs->Get("VBF_Hbb_JEC_dn");
  TH1F * vbfh_jer_up = (TH1F*)fs->Get("VBF_Hbb_JER_up");
  TH1F * vbfh_jer_dn = (TH1F*)fs->Get("VBF_Hbb_JER_dn");
  TH1F * vbfh_btg_up = (TH1F*)fs->Get("VBF_Hbb_BTG_up");
  TH1F * vbfh_btg_dn = (TH1F*)fs->Get("VBF_Hbb_BTG_dn");
  TH1F * vbfh_pu_up = (TH1F*)fs->Get("VBF_Hbb_PU_up");
  TH1F * vbfh_pu_dn = (TH1F*)fs->Get("VBF_Hbb_PU_dn");
  TH1F * vbfh_isr_up = (TH1F*)fs->Get("VBF_Hbb_PS-ISR_up");
  TH1F * vbfh_isr_dn = (TH1F*)fs->Get("VBF_Hbb_PS-ISR_dn");
  TH1F * vbfh_fsr_up = (TH1F*)fs->Get("VBF_Hbb_PS-FSR_up");
  TH1F * vbfh_fsr_dn = (TH1F*)fs->Get("VBF_Hbb_PS-FSR_dn");
  TH1F * vbfh_puid_up = (TH1F*)fs->Get("VBF_Hbb_PuJetID_up");
  TH1F * vbfh_puid_dn = (TH1F*)fs->Get("VBF_Hbb_PuJetID_dn");
  TH1F * vbfh_trig_up = (TH1F*)fs->Get("VBF_Hbb_OnlineBTG_up");
  TH1F * vbfh_trig_dn = (TH1F*)fs->Get("VBF_Hbb_OnlineBTG_dn");
  TH1F * ggh_jec_up = (TH1F*)fs->Get("ggF_Hbb_JEC_up");
  TH1F * ggh_jec_dn = (TH1F*)fs->Get("ggF_Hbb_JEC_dn");
  TH1F * ggh_jer_up = (TH1F*)fs->Get("ggF_Hbb_JER_up");
  TH1F * ggh_jer_dn = (TH1F*)fs->Get("ggF_Hbb_JER_dn");
  TH1F * ggh_btg_up = (TH1F*)fs->Get("ggF_Hbb_BTG_up");
  TH1F * ggh_btg_dn = (TH1F*)fs->Get("ggF_Hbb_BTG_dn");
  TH1F * ggh_pu_up = (TH1F*)fs->Get("ggF_Hbb_PU_up");
  TH1F * ggh_pu_dn = (TH1F*)fs->Get("ggF_Hbb_PU_dn");
  TH1F * ggh_isr_up = (TH1F*)fs->Get("ggF_Hbb_PS-ISR_up");
  TH1F * ggh_isr_dn = (TH1F*)fs->Get("ggF_Hbb_PS-ISR_dn");
  TH1F * ggh_fsr_up = (TH1F*)fs->Get("ggF_Hbb_PS-FSR_up");
  TH1F * ggh_fsr_dn = (TH1F*)fs->Get("ggF_Hbb_PS-FSR_dn");
  TH1F * ggh_puid_up = (TH1F*)fs->Get("ggF_Hbb_PuJetID_up");
  TH1F * ggh_puid_dn = (TH1F*)fs->Get("ggF_Hbb_PuJetID_dn");
  TH1F * ggh_trig_up = (TH1F*)fs->Get("ggF_Hbb_OnlineBTG_up");
  TH1F * ggh_trig_dn = (TH1F*)fs->Get("ggF_Hbb_OnlineBTG_dn");
  TH1F * tt_jec_up = (TH1F*)fs->Get("tt_JEC_up");
  TH1F * tt_jec_dn = (TH1F*)fs->Get("tt_JEC_dn");
  TH1F * tt_jer_up = (TH1F*)fs->Get("tt_JER_up");
  TH1F * tt_jer_dn = (TH1F*)fs->Get("tt_JER_dn");
  TH1F * tt_btg_up = (TH1F*)fs->Get("tt_BTG_up");
  TH1F * tt_btg_dn = (TH1F*)fs->Get("tt_BTG_dn");
  TH1F * tt_pu_up = (TH1F*)fs->Get("tt_PU_up");
  TH1F * tt_pu_dn = (TH1F*)fs->Get("tt_PU_dn");
  TH1F * tt_isr_up = (TH1F*)fs->Get("tt_PS-ISR_up");
  TH1F * tt_isr_dn = (TH1F*)fs->Get("tt_PS-ISR_dn");
  TH1F * tt_fsr_up = (TH1F*)fs->Get("tt_PS-FSR_up");
  TH1F * tt_fsr_dn = (TH1F*)fs->Get("tt_PS-FSR_dn");
  TH1F * tt_puid_up = (TH1F*)fs->Get("tt_PuJetID_up");
  TH1F * tt_puid_dn = (TH1F*)fs->Get("tt_PuJetID_dn");
  TH1F * tt_trig_up = (TH1F*)fs->Get("tt_OnlineBTG_up");
  TH1F * tt_trig_dn = (TH1F*)fs->Get("tt_OnlineBTG_dn");
  TH1F * zj_jec_up = (TH1F*)fs->Get("ZJets_JEC_up");
  TH1F * zj_jec_dn = (TH1F*)fs->Get("ZJets_JEC_dn");
  TH1F * zj_jer_up = (TH1F*)fs->Get("ZJets_JER_up");
  TH1F * zj_jer_dn = (TH1F*)fs->Get("ZJets_JER_dn");
  TH1F * zj_btg_up = (TH1F*)fs->Get("ZJets_BTG_up");
  TH1F * zj_btg_dn = (TH1F*)fs->Get("ZJets_BTG_dn");
  TH1F * zj_pu_up = (TH1F*)fs->Get("ZJets_PU_up");
  TH1F * zj_pu_dn = (TH1F*)fs->Get("ZJets_PU_dn");
  TH1F * vbfh_qgl = (TH1F*)fs->Get("VBF_Hbb_QGL_sys");
  TH1F * ggh_qgl = (TH1F*)fs->Get("ggF_Hbb_QGL_sys");
  TH1F * tt_qgl = (TH1F*)fs->Get("tt_QGL_sys");
  TH1F * zj_qgl = (TH1F*)fs->Get("ZJets_QGL_sys");
  TH1F * zj_isr_up = (TH1F*)fs->Get("ZJets_PS-ISR_up");
  TH1F * zj_isr_dn = (TH1F*)fs->Get("ZJets_PS-ISR_dn");
  TH1F * zj_fsr_up = (TH1F*)fs->Get("ZJets_PS-FSR_up");
  TH1F * zj_fsr_dn = (TH1F*)fs->Get("ZJets_PS-FSR_dn");
  TH1F * zj_puid_up = (TH1F*)fs->Get("ZJets_PuJetID_up");
  TH1F * zj_puid_dn = (TH1F*)fs->Get("ZJets_PuJetID_dn");
  TH1F * zj_trig_up = (TH1F*)fs->Get("ZJets_OnlineBTG_up");
  TH1F * zj_trig_dn = (TH1F*)fs->Get("ZJets_OnlineBTG_dn");

  for (int i=0; i<4; ++i) {

    //    RooRealVar * meanVar = wSig->var("mean_sig_"+names[i]);
    //    RooRealVar * sigmaVar = wSig->var("sigma_sig_"+names[i]);

    //    float mean = meanVar->getValV();
    //    float meanE = meanVar->getError();

    //    float sigma = sigmaVar->getValV();
    //    float sigmaE = sigmaVar->getError();

    RooRealVar * rate_ggH_var = wSig->var("norm_GGH_Nom_sig_"+names[i]);
    RooRealVar * rate_qqH_var = wSig->var("norm_VBF_Nom_sig_"+names[i]);
    RooRealVar * rate_tt_var = wtt->var("norm_Nom_tt_"+names[i]);
    RooRealVar * rate_zj_var = wzj->var("norm_Nom_zj_"+names[i]);
    
    double ggH_rate = rate_ggH_var->getValV();
    double qqH_rate = rate_qqH_var->getValV();
    double tt_rate = rate_tt_var->getValV();
    double zj_rate = rate_zj_var->getValV();

    ostringstream str;
    str << "datacards_bias/datacards_doubleb_" << names[i] << ".txt";
    string nn = str.str();
    const char * p = nn.c_str();
    std::ofstream textfile(p);
    textfile << "imax 1" << endl;
    textfile << "jmax *" << endl;
    textfile << "kmax *" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "shapes          qcd             *   DoubleBTag/root_shape/data_doubleb_shapes.root       w:qcd_$CHANNEL" << endl;
    textfile << "shapes          data_obs        *   DoubleBTag/root_shape/data_doubleb_shapes.root       w:data_$CHANNEL" << endl;
    textfile << "shapes          qqH_hbb         *   DoubleBTag/root_shape/signal_doubleb_shapes.root   w:qqH_$CHANNEL" << endl;
    textfile << "shapes          ggH_hbb         *   DoubleBTag/root_shape/signal_doubleb_shapes.root   w:ggH_$CHANNEL" << endl;
    textfile << "shapes          qqH_hbb_bias    *   DoubleBTag/root_shape/signal_doubleb_shapes.root   w:qqH_$CHANNEL" << endl;
    textfile << "shapes          ggH_hbb_bias    *   DoubleBTag/root_shape/signal_doubleb_shapes.root   w:ggH_$CHANNEL" << endl;
    textfile << "shapes          tt              *   DoubleBTag/root_shape/tt_doubleb_shapes.root  w:tt_$CHANNEL" << endl;
    textfile << "shapes          zj              *   DoubleBTag/root_shape/zj_doubleb_shapes.root  w:zj_$CHANNEL" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << endl;
    textfile << "observation   -1" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << "   " << names[i] << "   " << names[i] << "   " <<   names[i] << "   " << names[i] << "   " << names[i] << "   " << names[i] << endl;
    textfile << "process          ggH_hbb   qqH_hbb   qcd   tt    zj   ggH_hbb_bias  qqH_hbb_bias" << endl;
    textfile << "process            -1         0       1     2    3    4    5" << endl;
    //    textfile << "rate            " << ggH_rate << "  " << qqH_rate << " 1  " << tt_rate << "  " << zj_rate << endl;
    //    textfile << "rate     1     1    1    1    1   " << ggH_rate << "  " << qqH_rate << endl;
    textfile << "rate     1     1    1    1    1    1    1" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_BR_hbb     lnN	1.007/0.994	1.007/0.994	-      -     -   1.007/0.994     1.007/0.994" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    //    textfile << "lumi_13TeV_BBD_2018  lnN  -     -      -    -    -" << endl;
    textfile << "lumi_13TeV_BCC_2018  lnN  1.002    1.002    -     1.002   1.002   1.002   1.002" << endl;
    //    textfile << "lumi_13TeV_DB_2018   lnN  -     -      -    -    -" << endl;
    //    textfile << "lumi_13TeV_GS_2018   lnN  -     -      -    -    -" << endl;
    textfile << "lumi_13TeV_LS_2018   lnN  1.002    1.002    -    1.002   1.002   1.002   1.002" << endl;
    textfile << "lumi_13TeV_XY_2018   lnN  1.020    1.020    -    1.020   1.020   1.020   1.020" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << fixed << setprecision(3)<< "CMS_BTag_2018       lnN     " << (1 + ggh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_btg_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_btg_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_btg_up->GetBinContent(i+1)) << "/" <<  (1 + tt_btg_dn->GetBinContent(i+1)) << "      " << (1 + zj_btg_up->GetBinContent(i+1)) << "/" <<  (1 + zj_btg_dn->GetBinContent(i+1)) << "   " << (1 + ggh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_btg_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_btg_dn->GetBinContent(i+1))   << endl;
    textfile << fixed << setprecision(3)<< "CMS_PUweight_2018   lnN     " << (1 + ggh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_pu_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_pu_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_pu_up->GetBinContent(i+1)) << "/" <<  (1 + tt_pu_dn->GetBinContent(i+1)) << "      " << (1 + zj_pu_up->GetBinContent(i+1)) << "/" <<  (1 + zj_pu_dn->GetBinContent(i+1)) << "   " <<  (1 + ggh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_pu_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_pu_dn->GetBinContent(i+1))  << endl;
    textfile << fixed << setprecision(3)<< "CMS_QGLweight_2018  lnN     " << (1 + ggh_qgl->GetBinContent(i+1)) <<  "  " << (1 + vbfh_qgl->GetBinContent(i+1)) << "        -       " <<   (1 + tt_qgl->GetBinContent(i+1)) << "      " << (1 + zj_qgl->GetBinContent(i+1))  << "   " << (1 + ggh_qgl->GetBinContent(i+1)) <<  "  " << (1 + vbfh_qgl->GetBinContent(i+1)) << endl;
    textfile << fixed << setprecision(3)<< "CMS_PS_ISR_2018     lnN     " << (1 + ggh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_isr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_isr_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_isr_up->GetBinContent(i+1)) << "/" <<  (1 + tt_isr_dn->GetBinContent(i+1)) << "      " << (1 + zj_isr_up->GetBinContent(i+1)) << "/" <<  (1 + zj_isr_dn->GetBinContent(i+1))  << "   " <<  (1 + ggh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_isr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_isr_dn->GetBinContent(i+1)) << endl;
    textfile << fixed << setprecision(3)<< "CMS_PS_FSR_2018     lnN     " << (1 + ggh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_fsr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_fsr_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + tt_fsr_dn->GetBinContent(i+1)) << "      " << (1 + zj_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + zj_fsr_dn->GetBinContent(i+1)) << "   " <<  (1 + ggh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_fsr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_fsr_dn->GetBinContent(i+1))  << endl;
    textfile << fixed << setprecision(3)<< "CMS_Trig_2018     lnN     " << (1 + ggh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_trig_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_trig_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_trig_up->GetBinContent(i+1)) << "/" <<  (1 + tt_trig_dn->GetBinContent(i+1)) << "      " << (1 + zj_trig_up->GetBinContent(i+1)) << "/" <<  (1 + zj_trig_dn->GetBinContent(i+1)) << "   " << (1 + ggh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_trig_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_trig_dn->GetBinContent(i+1))  << endl;
    textfile << fixed << setprecision(3)<< "CMS_PileUpID_2018     lnN     " << (1 + ggh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_puid_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_puid_dn->GetBinContent(i+1))    << "        -       " <<   (1 + tt_puid_up->GetBinContent(i+1)) << "/" <<  (1 + tt_puid_dn->GetBinContent(i+1)) << "      " << (1 + zj_puid_up->GetBinContent(i+1)) << "/" <<  (1 + zj_puid_dn->GetBinContent(i+1)) << "   " <<  (1 + ggh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_puid_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_puid_dn->GetBinContent(i+1))  << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_norm_zj       lnN   -     -      -     -  1.3  -  -" << endl;
    textfile << "CMS_norm_tt       lnN   -     -      -   1.3    -  -  -" << endl;   
    textfile << "QCDscale_ggH      lnN  1.046/0.933   -    -    -    -  1.046/0.933   -" << endl;
    textfile << "PDFscale_ggH      lnN  1.019/0.981   -    -    -    -  1.019/0.981   -" << endl;
    textfile << "alphas_ggH        lnN  1.026/0.974   -    -    -    -  1.026/0.974   -" << endl;
    textfile << "QCDscale_qqH      lnN  -   1.004/0.997    -    -    -  -   1.004/0.997" << endl;
    textfile << "PDFscale_qqH      lnN  -   1.021/0.979    -    -    -  -   1.021/0.979" << endl;
    textfile << "alphas_qqH        lnN  -   1.005/0.995    -    -    -  -   1.005/0.995" << endl;
    textfile << "CMS_PDF_set       lnN    1.020   1.016  -    -    -  1.020  1.016" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_JES_2018  param 0.0 1.0" << endl;
    textfile << "CMS_JER_2018  param 0.0 1.0" << endl;
    //    textfile << "CMS_vbfbb_scale_mbb_13TeV_2018  param 1.0 0.015" << endl; 
    //    textfile << "CMS_vbfbb_res_mbb_13TeV_2018    param 1.0 0.035" << endl;
    //    textfile << "CMS_zj_scale_mbb_13TeV_2018  param 1.0 0.015" << endl; 
    //    textfile << "CMS_zj_res_mbb_13TeV_2018    param 1.0 0.035" << endl;
    //    textfile << "CMS_tt_scale_mbb_13TeV_2018  param 1.0 0.015" << endl; 
    //    textfile << "CMS_tt_res_mbb_13TeV_2018    param 1.0 0.035" << endl;
    //    textfile << "mean_sig_" << names[i] << "     param " << mean << " " << meanE << endl;
    //    textfile << "sigma_sig_" << names[i] << "    param " << sigma << " " << sigmaE << endl;
    //    textfile << "bkgNorm_" << names[i] << "  rateParam  " << names[i] << "   qcd   1   [0.5,1.5]" << endl;
    textfile << "bias_" << names[i] << " rateParam " << names[i] << " ggH_hbb_bias  0 [-1,1] " << endl; 
    textfile << "bias_" << names[i] << " rateParam " << names[i] << " qqH_hbb_bias  0 [-1,1] " << endl; 
    //    textfile << "bias_ggH_" << names[i] << " rateParam " << names[i] << " ggH_hbb_bias (@0*@1) " << "ggH_" << names[i] << "_norm,bias_" << names[i] << endl; 
    //    textfile << "bias_qqH_" << names[i] << " rateParam " << names[i] << " qqH_hbb_bias (@0*@1) " << "qqH_" << names[i] << "_norm,bias_" << names[i] << endl;
    textfile << "bias_" << names[i] << " param 0 0.2" << endl;
    textfile << endl;
  }

}
