#include "TH1F.h"

void createdatacards_bias()
{
Int_t CAT[] = {0,1,2,3,4,5,6};
for(int i=0; i < 7; i++)
{
	ostringstream str;
	str << "datacard_cat" << CAT[i] << ".txt";
	string nn = str.str();
        const char * p = nn.c_str();
        std::ofstream textfile(p);
	std::string filename = str.str();
	TFile  *f_ggh = new TFile("systematics/syst_ggH.root");
	TFile  *f_qqh = new TFile("systematics/syst_qqH.root");
	TFile  *f_zj  = new TFile("systematics/syst_zj.root");

	textfile << "imax 1" << endl;
        textfile << "jmax *" << endl;
        textfile << "kmax *" << endl;
textfile << "----------------------------------------------------------------------------------------------------" << endl;
textfile << "shapes          qcd             *   workspace/data_resolved_vbfhbb_2023.root       w:qcd_$CHANNEL" << endl;
textfile << "shapes          data_obs        *   workspace/data_resolved_vbfhbb_2023.root       w:data_$CHANNEL" << endl;
textfile << "shapes          qqH_hbb         *   workspace/signal_resolved_vbfhbb_2023.root   w:qqH_$CHANNEL" << endl;
textfile << "shapes          qqH_hbb_bias         *   workspace/signal_resolved_vbfhbb_2023.root   w:qqH_$CHANNEL" << endl;
textfile << "shapes          ggH_hbb         *   workspace/signal_resolved_vbfhbb_2023.root   w:ggH_$CHANNEL" << endl;
textfile << "shapes          ggH_hbb_bias         *   workspace/signal_resolved_vbfhbb_2023.root   w:ggH_$CHANNEL" << endl;
textfile << "shapes          zj              *   workspace/zj_resolved_vbfhbb_2023.root  w:zj_$CHANNEL" << endl;
textfile << "----------------------------------------------------------------------------------------------------" << endl;
textfile << "bin             DoubleB"<< CAT[i] << "_2023" << endl;
textfile << "observation   -1" << endl;
textfile << "----------------------------------------------------------------------------------------------------" << endl;
textfile << "bin             DoubleB" << CAT[i] << "_2023   DoubleB"<< CAT[i] << "_2023   DoubleB"<< CAT[i] << "_2023   DoubleB"<< CAT[i] << "_2023   DoubleB"<< CAT[i] << "_2023   DoubleB"<< CAT[i] << "_2023" <<   endl;
textfile << "process          ggH_hbb   qqH_hbb   qcd   zj   ggH_hbb_bias   qqH_hbb_bias" << endl;
textfile << "process            -1         0       1     2	3	4" << endl;
textfile << "rate     1     1    1    1    1    1" << endl;
textfile << "----------------------------------------------------------------------------------------------------" << endl;
textfile << "BR_hbb     lnN  1.007/0.994     1.007/0.994     -     -     1.007/0.994     1.007/0.994" << endl;
textfile << "lumi_13p6TeV   lnN  1.014    1.014    -   1.014     1.014    1.014" << endl;	
TString syst[] = {"CMS_pileup_2022EE",
                          "CMS_btag_lf_2022","CMS_btag_lfstats1_2022EE","CMS_btag_lfstats2_2022EE",
                          "CMS_btag_hf_2022","CMS_btag_hfstats1_2022EE","CMS_btag_hfstats2_2022EE",
                          "CMS_btag_cferr1_2022","CMS_btag_cferr2_2022"};
TString syst_name[] = {"CMS_pileup_2023",
                          "CMS_btag_lf_2023","CMS_btag_lfstats1_2023","CMS_btag_lfstats2_2023",
                          "CMS_btag_hf_2023","CMS_btag_hfstats1_2023","CMS_btag_hfstats2_2023",
                          "CMS_btag_cferr1_2023","CMS_btag_cferr2_2023"};
int nsyst = sizeof(syst)/sizeof(syst[0]);
int j;
if(i < 3) j = i;
else j = 0;
for(int kk=0; kk<nsyst; kk++)
            {
                TH1F *hggh_up =   (TH1F*)f_ggh->Get(syst[kk]+"Up"); TH1F *hggh_dn =   (TH1F*)f_ggh->Get(syst[kk]+"Down");
		TH1F *hqqh_up =   (TH1F*)f_qqh->Get(syst[kk]+"Up"); TH1F *hqqh_dn =   (TH1F*)f_qqh->Get(syst[kk]+"Down");
		TH1F *hzj_up  =   (TH1F*)f_zj->Get(syst[kk]+"Up"); TH1F *hzj_dn =   (TH1F*)f_zj->Get(syst[kk]+"Down");
		textfile <<  syst_name[kk]  << "  lnN   "  << hggh_up->GetBinContent(j+1) << "/" << hggh_dn->GetBinContent(j+1) << "	"  <<
		                          hqqh_up->GetBinContent(j+1) << "/" << hqqh_dn->GetBinContent(j+1) << "	"  <<	
					  "	-	" <<
					  hzj_up->GetBinContent(j+1) << "/" << hzj_dn->GetBinContent(j+1) << "	      "  <<  
                                          hggh_up->GetBinContent(j+1) << "/" << hggh_dn->GetBinContent(j+1) << "      "  <<
					  hqqh_up->GetBinContent(j+1) << "/" << hqqh_dn->GetBinContent(j+1)
					  << std::endl;
	    }
textfile << "----------------------------------------------------------------------------------------------------" << endl;
textfile << "PS_ISR             lnN     1.054/0.935   1.005/0.993   -  1.145/0.825    1.054/0.935   1.005/0.993" << endl;
textfile << "PS_FSR             lnN     1.149/1.037   1.053/0.943   -   1.119/0.84    1.149/1.037   1.053/0.943" << endl;
textfile << "CMS_vbfhbb_Trig_2023     lnN     1.15/0.85  1.15/0.85    -   1.15/0.85   1.15/0.85  1.15/0.85" << endl;
textfile << "CMS_zj_d1K_NLO     lnN      -	-	-	1.064    -      -" << endl;
textfile << "CMS_zj_d1kappa_EW   lnN      -      -       -       1.001    -      -" << endl;
textfile << "CMS_zj_d2K_NLO     lnN     -      -       -       1.056     -      -" << endl;
textfile << "CMS_zj_d2kappa_EW  lnN     -      -       -       1.069     -      -" << endl; 
textfile << "CMS_zj_d3K_NLO     lnN     -      -       -       1.001     -      -" << endl;
textfile << "CMS_zj_d3kappa_EW  lnN     -      -       -       1.001     -      -" << endl;
textfile << "VBF_Dipole_Recoil	lnN	-   1.14/0.86   -   -   -  1.14/0.86" << endl;
textfile << "QCDscale_ggH      lnN  1.046/0.933   -    -   -   1.046/0.933  -" << endl;
textfile << "pdf_gg      lnN  1.019/0.981   -    -   -  1.019/0.981 -" << endl;
textfile << "alphas_ggH        lnN  1.026/0.974   -    -    -     1.026/0.974   -   " << endl;
textfile << "QCDscale_qqH      lnN  -   1.004/0.997    -    -     -   1.004/0.997  " << endl;
textfile << "pdf_qqbar      lnN  -   1.021/0.979    -    -      -   1.021/0.979 " << endl;
textfile << "alphas_qqH        lnN  -   1.005/0.995    -    -  -   1.005/0.995" << endl;
textfile << "----------------------------------------------------------------------------------------------------" << endl;
textfile << "CMS_JES_2023  param 0.0 1.0" << endl;
textfile << "CMS_JER_2023  param 0.0 1.0" << endl;
textfile << "CMS_scale_b_2023  param 0.0 1.0" << endl;
textfile << "CMS_res_b_2023  param 0.0 1.0" << endl;
textfile << "bias_DoubleB" << CAT[i] << "_2023	param  0 0.2" << endl;
textfile << "bias_DoubleB" << CAT[i] << "_2023	rateParam	DoubleB" << CAT[i] << "_2023   ggH_hbb_bias 0 [-1,1]" << endl;
textfile << "bias_DoubleB" << CAT[i] << "_2023  rateParam       DoubleB" << CAT[i] << "_2023   qqH_hbb_bias 0 [-1,1]" << endl;
textfile << "normZ rateParam DoubleB"<<CAT[i] <<"_2023 zj 1 [-20,20]" << endl;
f_ggh->Close();
f_qqh->Close();
f_zj->Close();
 }
}

