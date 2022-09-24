#include "Common.h"
// TF = true  : background model with the transfer functions
// TF = false : background model based on Bernstein polynomials, QCD spectra are fit independently in different categories 
double hard_sysval[] = {1.14,1.02,1.20,1.16,1.45};
double dipole_sysval[6] = {-0.1543763086,-0.2367654592,0.1558055229,0.01458640275,0.01682894227,0.02935335761};
void CreateDatacards_bias(bool freeZnorm = true,
			  TString folder = ""){

  using namespace RooFit;

  TString suffix("_2016");


  TFile  *fs = new TFile("VBFHbb_2016_DoubleB_Systematics.root");

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
  TH1F * vbfh_l1pf_up = (TH1F*)fs->Get("VBF_Hbb_Dipol_L1PF_up");
  TH1F * vbfh_l1pf_dn = (TH1F*)fs->Get("VBF_Hbb_Dipol_L1PF_dn");


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
  TH1F * ggh_l1pf_up = (TH1F*)fs->Get("ggF_Hbb_POWHEG_L1PF_up");
  TH1F * ggh_l1pf_dn = (TH1F*)fs->Get("ggF_Hbb_POWHEG_L1PF_dn");


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
  TH1F * zj_pu_up =   (TH1F*)fs->Get("ZJets_PU_up");
  TH1F * zj_pu_dn =  (TH1F*)fs->Get("ZJets_PU_dn");
  TH1F * zj_isr_up = (TH1F*)fs->Get("ZJets_ISR_up");
  TH1F * zj_isr_dn = (TH1F*)fs->Get("ZJets_ISR_dn");
  TH1F * zj_fsr_up = (TH1F*)fs->Get("ZJets_FSR_up");
  TH1F * zj_fsr_dn = (TH1F*)fs->Get("ZJets_FSR_dn");
  TH1F * zj_puid_up = (TH1F*)fs->Get("ZJets_PUID_up");
  TH1F * zj_puid_dn = (TH1F*)fs->Get("ZJets_PUID_dn");
  TH1F * zj_trig_up = (TH1F*)fs->Get("ZJets_TRIG_up");
  TH1F * zj_trig_dn = (TH1F*)fs->Get("ZJets_TRIG_dn");
  TH1F * zj_qgl     = (TH1F*)fs->Get("ZJets_QGL_up");
  TH1F * zj_l1pf_up = (TH1F*)fs->Get("ZJets_L1PF_up");
  TH1F * zj_l1pf_dn = (TH1F*)fs->Get("ZJets_L1PF_dn");

  TFile *f = new TFile("VBFHBB_2016_DoubleB_ZJets_NLO_kfact_syst_unc.root");
  TH1F *d1K_NLO_up = (TH1F*)f->Get("ZJets_d1K_NLO_up");
  TH1F *d1K_NLO_dn = (TH1F*)f->Get("ZJets_d1K_NLO_dn");
  TH1F *d2K_NLO_up = (TH1F*)f->Get("ZJets_d2K_NLO_up");
  TH1F *d2K_NLO_dn = (TH1F*)f->Get("ZJets_d2K_NLO_dn");
  TH1F *d3K_NLO_up = (TH1F*)f->Get("ZJets_d3_kNLO_up");
  TH1F *d3K_NLO_dn = (TH1F*)f->Get("ZJets_d3_kNLO_dn");
  TH1F *Z_d1kappa_EW_up = (TH1F*)f->Get("ZJets_d1kappa_EW_up");
  TH1F *Z_d1kappa_EW_dn = (TH1F*)f->Get("ZJets_d1kappa_EW_dn");
  TH1F *Z_d2kappa_EW_up = (TH1F*)f->Get("ZJets_Z_d2kappa_EW_up");
  TH1F *Z_d2kappa_EW_dn = (TH1F*)f->Get("ZJets_Z_d2kappa_EW_dn");
  TH1F *Z_d3kappa_EW_up = (TH1F*)f->Get("ZJets_Z_d3kappa_EW_up");
  TH1F *Z_d3kappa_EW_dn = (TH1F*)f->Get("ZJets_Z_d3kappa_EW_dn");


  TH1F * vbfh_diploe = (TH1F*)fs->Get("VBF_Hbb_Dipole_RecoilOn_up");
  for (int i=0; i<6; ++i) {

    //    RooRealVar * meanVar = wSig->var("mean_sig_"+names[i]);
    //    RooRealVar * sigmaVar = wSig->var("sigma_sig_"+names[i]);

    //    float mean = meanVar->getValV();
    //    float meanE = meanVar->getError();

    //    float sigma = sigmaVar->getValV();
    //    float sigmaE = sigmaVar->getError();

    ostringstream str;
    str << "datacards/datacards_doubleb_" << names[i] << "_Preapproval.txt";
    string nn = str.str();
    const char * p = nn.c_str();
    std::ofstream textfile(p);
    textfile << "imax 1" << endl;
    textfile << "jmax *" << endl;
    textfile << "kmax *" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "shapes          qcd             *   " << folder << "workspace/data_doubleb_shapes"+suffix+".root       w:qcd_$CHANNEL" << endl;
    textfile << "shapes          data_obs        *   " << folder << "workspace/data_doubleb_shapes"+suffix+".root       w:data_$CHANNEL" << endl;
    textfile << "shapes          qqH_hbb         *   " << folder << "workspace/signal_doubleb_shapes"+suffix+".root   w:qqH_$CHANNEL" << endl;
    textfile << "shapes          ggH_hbb         *   " << folder << "workspace/signal_doubleb_shapes"+suffix+".root   w:ggH_$CHANNEL" << endl;
    textfile << "shapes          qqH_hbb_bias    *   " << folder << "workspace/signal_doubleb_shapes"+suffix+".root   w:qqH_$CHANNEL" << endl;
    textfile << "shapes          ggH_hbb_bias    *   " << folder << "workspace/signal_doubleb_shapes"+suffix+".root   w:ggH_$CHANNEL" << endl;
    textfile << "shapes          zj              *   " << folder << "workspace/zj_doubleb_shapes"+suffix+".root  w:zj_$CHANNEL" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << endl;
    textfile << "observation   -1" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "bin             " << names[i] << "   " << names[i] << "   " << names[i] << "   " << names[i] << "   " << names[i] << "    " << names[i] << endl;
    textfile << "process          ggH_hbb   qqH_hbb   qcd   zj   ggH_hbb_bias  qqH_hbb_bias" << endl;
    textfile << "process            -1         0       1     2    3     4        " << endl;
    textfile << "rate     1     1    1    1    1    1 " << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "BR_hbb     lnN	1.007/0.994	1.007/0.994	-     -     1.007/0.994     1.007/0.994" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;

    textfile << "lumi_13TeV_corr   lnN  1.020    1.020    -    1.020   1.020    1.020" << endl;
    textfile << "lumi_13TeV_2016   lnN  1.010    1.010    -    1.010   1.010    1.010" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    //Offline BTAG
    textfile << fixed << setprecision(3)<< "CMS_btag_comb_2016       lnN     " << (1 + ggh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_btg_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_btg_dn->GetBinContent(i+1))    << "        -       " << (1 + zj_btg_up->GetBinContent(i+1)) << "/" <<  (1 + zj_btg_dn->GetBinContent(i+1)) << "   " << (1 + ggh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_btg_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_btg_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_btg_dn->GetBinContent(i+1))   << endl;
    //PU - reweighting
   textfile << fixed << setprecision(3)<< "CMS_pileup_2016   lnN     " << (1 + ggh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_pu_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_pu_dn->GetBinContent(i+1))    << "        -       " << (1 + zj_pu_up->GetBinContent(i+1)) << "/" <<  (1 + zj_pu_dn->GetBinContent(i+1)) << "   "  <<  (1 + ggh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_pu_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_pu_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_pu_dn->GetBinContent(i+1))  << endl;
    //QGL - reweighting
    textfile << fixed << setprecision(3)<< "CMS_vbfhbb_QGLweight_2016  lnN     " << (1 + ggh_qgl->GetBinContent(i+1)) <<  "  " << (1 + vbfh_qgl->GetBinContent(i+1)) << "        -       "  << (1 + zj_qgl->GetBinContent(i+1))  << "   "  << (1 + ggh_qgl->GetBinContent(i+1)) <<  "  " << (1 + vbfh_qgl->GetBinContent(i+1)) << endl;
    //ISR
    textfile << fixed << setprecision(3)<< "PS_ISR     lnN     " << (1 + ggh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_isr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_isr_dn->GetBinContent(i+1))    << "        -       " <<  (1 + zj_isr_up->GetBinContent(i+1)) << "/" <<  (1 + zj_isr_dn->GetBinContent(i+1))  << "   " << (1 + ggh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_isr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_isr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_isr_dn->GetBinContent(i+1)) << endl;
    //FSR
    textfile << fixed << setprecision(3)<< "PS_FSR     lnN     " << (1 + ggh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_fsr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_fsr_dn->GetBinContent(i+1))    << "        -       " <<   (1 + zj_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + zj_fsr_dn->GetBinContent(i+1)) << "   " <<  (1 + ggh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_fsr_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_fsr_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_fsr_dn->GetBinContent(i+1))  << endl;
    //Online TRIG
    textfile << fixed << setprecision(3)<< "CMS_vbfhbb_Trig_2016     lnN     " << (1 + ggh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_trig_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_trig_dn->GetBinContent(i+1))    << "        -       " <<   (1 + zj_trig_up->GetBinContent(i+1)) << "/" <<  (1 + zj_trig_dn->GetBinContent(i+1)) << "   " << (1 + ggh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_trig_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_trig_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_trig_dn->GetBinContent(i+1))  << endl;
    //PUID
    textfile << fixed << setprecision(3)<< "CMS_PileUpID_2016     lnN     " << (1 + ggh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_puid_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_puid_dn->GetBinContent(i+1))    << "        -       " << (1 + zj_puid_up->GetBinContent(i+1)) << "/" <<  (1 + zj_puid_dn->GetBinContent(i+1)) << "   " << (1 + ggh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_puid_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_puid_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_puid_dn->GetBinContent(i+1))  << endl;
    //    Dipole
    textfile << fixed << setprecision(3)<< "VBF_Dipole_Recoil   lnN     " << "  -       " << (1 + dipole_sysval[i]) << "   -   -   -  " << (1 + dipole_sysval[i]) << endl;
    //L1PreFire
    textfile << fixed << setprecision(3)<< "CMS_L1Prefire_2016  lnN     " << (1 + ggh_l1pf_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_l1pf_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_l1pf_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_l1pf_dn->GetBinContent(i+1))    << "        -       " << (1 + zj_l1pf_up->GetBinContent(i+1)) << "/" <<  (1 + zj_l1pf_dn->GetBinContent(i+1)) << "   " << (1 + ggh_l1pf_up->GetBinContent(i+1)) << "/" <<  (1 + ggh_l1pf_dn->GetBinContent(i+1)) << "  " << (1 + vbfh_l1pf_up->GetBinContent(i+1)) << "/" <<  (1 + vbfh_l1pf_dn->GetBinContent(i+1))  << endl;
    //NLO k-factor
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << fixed << setprecision(3)<< "CMS_zj_d1K_NLO   lnN        -       -       -       " << 1 + d1K_NLO_up->GetBinContent(i+1)+0.001 << "   -       -       " <<endl;
    textfile << fixed << setprecision(3)<< "CMS_zj_d2K_NLO   lnN        -       -       -       " << 1 + d2K_NLO_up->GetBinContent(i+1)+0.001 << "       -       -       " <<endl;
    textfile << fixed << setprecision(3)<< "CMS_zj_d3K_NLO   lnN        -       -       -       " << 1 + d3K_NLO_up->GetBinContent(i+1)+0.001 << "       -       -       " <<endl;

    textfile << fixed << setprecision(3)<< "CMS_zj_d1kappa_EW   lnN        -       -       -       " << 1 + Z_d1kappa_EW_up->GetBinContent(i+1)+0.001 << "       -       -       " <<endl;
    textfile << fixed << setprecision(3)<< "CMS_zj_d2kappa_EW   lnN        -       -       -       " << 1 + Z_d2kappa_EW_up->GetBinContent(i+1)+0.001 << "       -       -       " <<endl;
    textfile << fixed << setprecision(3)<< "CMS_zj_d3kappa_EW   lnN        -       -       -       " << 1 + Z_d3kappa_EW_up->GetBinContent(i+1)+0.001 << "       -       -       " <<endl;

    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "QCDscale_ggH      lnN  1.046/0.933   -    -    -	1.046/0.933   -" << endl;
    textfile << "pdf_gg      lnN  1.019/0.981   -    -	-	1.019/0.981   -" << endl;
    textfile << "alphas_ggH        lnN  1.026/0.974   -    -    -	1.026/0.974   -" << endl;
    textfile << "QCDscale_qqH      lnN  -   1.004/0.997    -    -       -   1.004/0.997" << endl;
    textfile << "pdf_qqbar      lnN  -   1.021/0.979    -    -       -   1.021/0.979" << endl;
    textfile << "alphas_qqH        lnN  -   1.005/0.995    -    -       -   1.005/0.995" << endl;
    textfile << "----------------------------------------------------------------------------------------------------" << endl;
    textfile << "CMS_JES_2016  param 0.0 1.0" << endl;
    textfile << "CMS_JER_2016  param 0.0 1.0" << endl;
    textfile << "CMS_scale_b_2016  param 0.0 1.0" << endl;
    textfile << "CMS_res_b_2016  param 0.0 1.0" << endl;
    textfile << "bias_" << names[i] << " rateParam " << names[i] << " ggH_hbb_bias  0 [-1,1] " << endl; 
    textfile << "bias_" << names[i] << " rateParam " << names[i] << " qqH_hbb_bias  0 [-1,1] " << endl; 
    textfile << "bias_" << names[i] << " param 0 0.2" << endl;
    textfile << "normZ" << " rateParam " << names[i] << " zj 1 [-20,20]" << endl;
    if (!freeZnorm) textfile << "normZ" << " param 1.0 0.5" << endl;
    textfile << endl;

  }

}
