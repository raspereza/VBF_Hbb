#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

void myPlotter()
{
/*	
   TString Var_Name[] = {"PT1","PT2","PT3","PT4","ETAb1","ETAb2","ETAq1","ETAq2","PTb1","PTb2","PTq1","PTq2","MINVQQ","DETAQQ",
                         "BTG1_DeepCSV","BTG1_DeepJet","BTG2_DeepCSV","BTG2_DeepJet","MINVBB",
                         "Q1_QTG","Q2_QTG","dPHI_QQ","NJ20","NJ30","NJ_InGap","Alpha_B1H","Alpha_Q1QQ","DETA_BB_QQ","PT5","ZEP","PHIA_QQ_BB","MIN_R_B_Q","DR_Q1_BB","DR_Q2_BB","Norm_PT","HTT_rest_20",
			 "E_rest_20","pzAll"
                        };

   int Nbin[]         = { 50,  50,  50,  50,   94,   94,   94,   94,  50,  50,  50,  50,  100, 100, 100, 100, 100, 100, 100, 100, 100,  32, 10, 10, 10,  35,  35,   70,  50,   70,  35,  40, 50, 70,  60, 10, 20, 120};
   double Xmin[]      = { 80,  70,  60,  20, -4.7, -4.7, -4.7, -4.7,  30,  30,  30,  30,  400, 3.5,   0,   0,   0,   0,   0,  -1,  -1,   0,  4,  4,  0,   0,   0, -3.5,   0, -3.5,   0, 0.5,  1,  0,   0, 0, 0, -6000.0};
   double Xmax[]      = {480, 370, 310, 220,  4.7,  4.7,  4.7,  4.7, 330, 330, 330, 330, 3000,  10,   1,   1,   1,   1, 500,   1,   1, 3.2, 14, 14, 10, 3.5, 3.5,  3.5, 150,  3.5, 3.5, 4.5,  6,  7, 0.6, 100, 200, 6000.0};
   int nVar = sizeof(Var_Name)/sizeof(Var_Name[0]);
*/   
         /*         
	 TString Var_Name[] = {"PT1",  "PT2",  "PT3",  "PT4", "BTG1_DeepJet","BTG2_DeepJet", "MINVQQ",  "DETAQQ"};
	 int Nbin[]         = { 25,     25,     25,     25,       50,           50,              50,        50};
	 double Xmin[]      = { 0.0,    0.0,   0.0,     0.0,     0.0,          0.0,             0.0,       0.0};
	 double Xmax[]      = { 500.0,300.0, 300.0,   200.0,    1.0,          1.0,           2500.0,       10.0};
         */
	 /*
         TString Var_Name[] = {"ETA1",  "ETA2",  "ETA3",  "ETA4"};
         int Nbin[]         = {   50,     50,      50,      50};
	 double Xmin[]      = { -5.0,   -5.0,   -5.0,    -5.0  };
         double Xmax[]      = {   5.0,    5.0,    5.0,     5.0  };
	 */ 
         /* 
    	 TString Var_Name[] = {"PTQ1" ,  "PTQ2",  "PTB1",  "PTB2",  "MQQ",  "MBB",  };
         int Nbin[]         = { 25,       25,       25,      25,     50,      10 ,  };
         double Xmin[]      = { 0.0,     0.0,   0.0,     0.0,       0.0,    124.0,  };
         double Xmax[]      = { 200.0, 200.0, 200.0,   200.0,    2500.0,    126.0,  };
         */ 
         /*
         TString Var_Name[] = {"ptQ1" ,  "ptQ2",  "ptB1",  "ptB2"};
	 int Nbin[]         = { 25,       25,       25,      25};
         double Xmin[]      = { 0.0,     0.0,   0.0,     0.0  };
         double Xmax[]      = { 200.0, 200.0, 200.0,   200.0  };
	 */ 
	 /*
         TString Var_Name[] = {"pt1" ,  "pt2",  "pt3",  "pt4"};
         int Nbin[]         = { 25,       25,       25,      25};
         double Xmin[]      = { 0.0,     0.0,   0.0,     0.0  };
         double Xmax[]      = { 200.0, 200.0, 200.0,   200.0  };
         */ 
	 /* 
	 TString Var_Name[] = {"PTb1","PTb2","PTq1","PTq2"};
         int Nbin[]         = { 25,       25,    25,      25  };
         double Xmin[]      = { 0.0,     0.0,   0.0,     0.0  };
         double Xmax[]      = { 300.0, 300.0, 300.0,   300.0  };
         */         
	 /*
  	 TString Var_Name[] = {"Njet_20","Njet_20_ctrl","Njet_30","Njet_30_ctrl"};
         int Nbin[]         = { 10,       10,    10,      10  };
         double Xmin[]      = { 0.0,     0.0,   0.0,     0.0  };
         double Xmax[]      = { 10.0,   10.0,  10.0,    10.0  };
         */
          TString Var_Name[] = {"bdtout"};
          int Nbin[]         = {25  };
          double Xmin[]      = {0.0  };
          double Xmax[]      = {1.0  };

	 
	 int nVar = sizeof(Var_Name)/sizeof(Var_Name[0]);
	 Float_t rebin =1;
for(int variable = 0; variable < nVar; variable++)
 {
// ***************************************************************************************************************
// Signal
   std::cout << Var_Name[variable] << std::endl;

   TFile *f01 = TFile::Open("ntuples/mbb_and_bdt_all_Nom.root");
   TFile *f00 = TFile::Open("../DoubleBTag_2018_UL_Preapproval/ntuples/mbb_and_bdt_all_Nom.root");
   

   TTree * ntps[2]    = {(TTree*)f00->Get("Mass_and_BDT_ZJets"), (TTree*)f01->Get("Mass_and_BDT_ZJets")};
   int nSProc = sizeof(ntps)/sizeof(ntps[0]);

   Float_t MyS_weight[nSProc],MyS_Var[nSProc];
   for(int k=0;k<nSProc;k++)
   {
    ntps[k]->SetBranchAddress( "weight",     &MyS_weight[k] );
    if(Var_Name[variable]=="PT1")
      ntps[k]->SetBranchAddress( "T_pt0",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PT2")
      ntps[k]->SetBranchAddress( "T_pt1",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PT3")
      ntps[k]->SetBranchAddress( "T_pt2",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PT4")
      ntps[k]->SetBranchAddress( "T_pt3",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ETAb1")
      ntps[k]->SetBranchAddress( "T_etab1",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ETAb2")
      ntps[k]->SetBranchAddress( "T_etab2",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ETAq1")
      ntps[k]->SetBranchAddress( "T_etaq1",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ETAq2")
      ntps[k]->SetBranchAddress( "T_etaq2",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PTb1")
      ntps[k]->SetBranchAddress( "T_ptb1",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PTb2")
      ntps[k]->SetBranchAddress( "T_ptb2",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PTq1")
      ntps[k]->SetBranchAddress( "T_ptq1",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PTq2")
      ntps[k]->SetBranchAddress( "T_ptq2",   &MyS_Var[k] );
    else if(Var_Name[variable]=="MINVQQ")
      ntps[k]->SetBranchAddress( "T_mqq",      &MyS_Var[k] );
    else if(Var_Name[variable]=="DETAQQ")
      ntps[k]->SetBranchAddress( "T_dETAqq",   &MyS_Var[k] );
    else if(Var_Name[variable]=="BTG1_DeepCSV")
      ntps[k]->SetBranchAddress( "T_btgb1_DeepCSV",  &MyS_Var[k] );
    else if(Var_Name[variable]=="BTG1_DeepJet")
      ntps[k]->SetBranchAddress( "T_btgb1_DeepJet",  &MyS_Var[k] );
    else if(Var_Name[variable]=="BTG2_DeepCSV")
      ntps[k]->SetBranchAddress( "T_btgb2_DeepCSV",  &MyS_Var[k] );
    else if(Var_Name[variable]=="BTG2_DeepJet")
      ntps[k]->SetBranchAddress( "T_btgb2_DeepJet",  &MyS_Var[k] );
    else if(Var_Name[variable]=="MINVBB")
      ntps[k]->SetBranchAddress( "T_mbb",   &MyS_Var[k] );
    else if(Var_Name[variable]=="Q1_QTG")
      ntps[k]->SetBranchAddress( "T_qtgq1",   &MyS_Var[k] );
    else if(Var_Name[variable]=="Q2_QTG")
      ntps[k]->SetBranchAddress( "T_qtgq2",   &MyS_Var[k] );
    else if(Var_Name[variable]=="dPHI_QQ")
      ntps[k]->SetBranchAddress( "T_dPHIqq",   &MyS_Var[k] );
    else if(Var_Name[variable]=="NJ20")
      ntps[k]->SetBranchAddress( "T_NJ_20",   &MyS_Var[k] );
    else if(Var_Name[variable]=="NJ30")
      ntps[k]->SetBranchAddress( "T_NJ_30",   &MyS_Var[k] );
    else if(Var_Name[variable]=="NJ_InGap")
      ntps[k]->SetBranchAddress( "T_NJ_ingap",   &MyS_Var[k] );
    else if(Var_Name[variable]=="Alpha_B1H")
      ntps[k]->SetBranchAddress( "T_alphabb",   &MyS_Var[k] );
    else if(Var_Name[variable]=="Alpha_Q1QQ")
      ntps[k]->SetBranchAddress( "T_alphaqq",   &MyS_Var[k] );
    else if(Var_Name[variable]=="DETA_BB_QQ")
      ntps[k]->SetBranchAddress( "T_dETA_bb_qq",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PT5")
      ntps[k]->SetBranchAddress( "T_pt4",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ZEP")
      ntps[k]->SetBranchAddress( "T_zep",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PHIA_QQ_BB")
      ntps[k]->SetBranchAddress( "T_phiA_bb_qq",   &MyS_Var[k] );
    else if(Var_Name[variable]=="MIN_R_B_Q")
      ntps[k]->SetBranchAddress( "T_minR",   &MyS_Var[k] );
    else if(Var_Name[variable]=="DR_Q1_BB")
      ntps[k]->SetBranchAddress( "T_dR_leadqH",   &MyS_Var[k] );
    else if(Var_Name[variable]=="DR_Q2_BB")
      ntps[k]->SetBranchAddress( "T_dR_subleadqH",   &MyS_Var[k] );
    else if(Var_Name[variable]=="Norm_PT")
      ntps[k]->SetBranchAddress( "T_norm_pt",   &MyS_Var[k] );
    else if(Var_Name[variable]=="HTT_rest_20")
      ntps[k]->SetBranchAddress( "T_HTT_rest_20",   &MyS_Var[k] );
    else if(Var_Name[variable]=="E_rest_20")
      ntps[k]->SetBranchAddress( "T_E_rest_20",   &MyS_Var[k] );
    else if(Var_Name[variable]=="pzAll")
      ntps[k]->SetBranchAddress( "T_pzAll",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PTQ1")
      ntps[k]->SetBranchAddress( "T_q1pt",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PTQ2")
      ntps[k]->SetBranchAddress( "T_q2pt",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PTB1")
      ntps[k]->SetBranchAddress( "T_b1pt",   &MyS_Var[k] );
    else if(Var_Name[variable]=="PTB2")
      ntps[k]->SetBranchAddress( "T_b2pt",   &MyS_Var[k] );
    else if(Var_Name[variable]=="MQQ")
      ntps[k]->SetBranchAddress( "T_Mqq",   &MyS_Var[k] );
    else if(Var_Name[variable]=="MBB")
      ntps[k]->SetBranchAddress( "T_Mbb",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ptQ1")
      ntps[k]->SetBranchAddress( "T_q1jpt",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ptQ2")
      ntps[k]->SetBranchAddress( "T_q2jpt",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ptB1")
      ntps[k]->SetBranchAddress( "T_b1jpt",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ptB2")
      ntps[k]->SetBranchAddress( "T_b2jpt",   &MyS_Var[k] );
    else if(Var_Name[variable]=="pt1")
      ntps[k]->SetBranchAddress( "T_genpt0",   &MyS_Var[k] );
    else if(Var_Name[variable]=="pt2")
      ntps[k]->SetBranchAddress( "T_genpt1",   &MyS_Var[k] );
    else if(Var_Name[variable]=="pt3")
      ntps[k]->SetBranchAddress( "T_genpt2",   &MyS_Var[k] );
    else if(Var_Name[variable]=="pt4")
      ntps[k]->SetBranchAddress( "T_genpt3",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ETA1")
      ntps[k]->SetBranchAddress( "T_eta0",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ETA2")
      ntps[k]->SetBranchAddress( "T_eta1",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ETA3")
      ntps[k]->SetBranchAddress( "T_eta2",   &MyS_Var[k] );
    else if(Var_Name[variable]=="ETA4")
      ntps[k]->SetBranchAddress( "T_eta3",   &MyS_Var[k] );
    else if(Var_Name[variable]=="Njet_20")
      ntps[k]->SetBranchAddress( "T_Njet_20",   &MyS_Var[k] );
    else if(Var_Name[variable]=="Njet_30")
      ntps[k]->SetBranchAddress( "T_Njet_30",   &MyS_Var[k] );
    else if(Var_Name[variable]=="Njet_20_ctrl")
      ntps[k]->SetBranchAddress( "T_Njet_20_ctrl",   &MyS_Var[k] );
    else if(Var_Name[variable]=="Njet_30_ctrl")
      ntps[k]->SetBranchAddress( "T_Njet_30_ctrl",   &MyS_Var[k] );
    else if(Var_Name[variable]=="bdtout")
      ntps[k]->SetBranchAddress( "bdtout_1",   &MyS_Var[k] );

   }

  TString Name = Var_Name[variable];
  int nbin=Nbin[variable];
  double xmin=Xmin[variable];
  double xmax=Xmax[variable];

  TH1D * _Var_VBF_Hbb  = new TH1D(Name+"_VBF_Hbb",Name+"_VBF_Hbb",nbin,xmin,xmax);
  TH1D * _Var_VBF_Hbb_H   = new TH1D(Name+"_VBF_Hbb_H",Name+"_DATA",nbin,xmin,xmax);
  


  for(int k=0;k<nSProc;k++)
   for(int i=0;i<ntps[k]->GetEntries();i++)
    {
     ntps[k]    -> GetEntry(i);
     if(k==0)
      _Var_VBF_Hbb->Fill(MyS_Var[k],MyS_weight[k]);
     if(k==1)
      _Var_VBF_Hbb_H->Fill(MyS_Var[k],MyS_weight[k]);
    }
  //_Var_VBF_Hbb->Scale(1/_Var_VBF_Hbb->Integral());
  //_Var_VBF_Hbb_H->Scale(1/_Var_VBF_Hbb_H->Integral());

  _Var_VBF_Hbb_H->SetBinContent(14, 1.05 * _Var_VBF_Hbb_H->GetBinContent(14)); 
  for(int ii = 15; ii < 26 ; ii++)
  {
	   _Var_VBF_Hbb_H->SetBinContent(ii, 1.1 * _Var_VBF_Hbb_H->GetBinContent(ii));
  }

  Float_t xVal = rebin*(xmax-xmin)/nbin;
  TString xtitle = "Events / ";
  if (Var_Name[variable] == "PT1" || Var_Name[variable] == "PT2" || Var_Name[variable] == "PT3" || Var_Name[variable] == "PT4"|| Var_Name[variable] == "PT5"|| Var_Name[variable] == "PTb1" || Var_Name[variable] == "PTb2" || Var_Name[variable] == "PTq1" || Var_Name[variable] == "PTq2" || Var_Name[variable] == "MINVQQ" || Var_Name[variable] == "MINVBB" || Var_Name[variable] =="Mass_in_BDT" ||  Var_Name[variable] =="pzAll" || Var_Name[variable] == "PTQ1" || Var_Name[variable] == "PTQ2" || Var_Name[variable] == "PTB1" || Var_Name[variable] == "PTB2" || Var_Name[variable] == "MBB" || Var_Name[variable] == "MQQ" || Var_Name[variable] == "ptQ1" || Var_Name[variable] == "ptQ2" || Var_Name[variable] == "ptB1" || Var_Name[variable] == "ptB2" || Var_Name[variable] == "pt1" || Var_Name[variable] == "pt2" || Var_Name[variable] == "pt3" || Var_Name[variable] == "pt4") 
	{
         xtitle+=Form("%.1f",xVal);
         xtitle+=" GeV";
        }
      else
        xtitle+=Form("%.2f",xVal);

  _Var_VBF_Hbb->GetXaxis()->SetTitle(Var_Name[variable]);
  //T_mqq,T_dETAqq,T_dPHIqq,T_btgb1_DeepJet,T_btgb2_DeepJet,T_qtgq1,T_qtgq2,T_pzAll,T_NJ_20,T_E_rest_20,T_HTT_rest_20,T_alphaqq,T_phiA_bb_qq,T_dR_subleadqH,T_norm_pt; 
  //TString Var_Name[] = {"PT1","PT2","PT3","PT4","ETAb1","ETAb2","ETAq1","ETAq2","PTb1","PTb2","PTq1","PTq2","MINVQQ","DETAQQ",
  //                       "BTG1_DeepCSV","BTG1_DeepJet","BTG2_DeepCSV","BTG2_DeepJet","MINVBB",
  //                       "Q1_QTG","Q2_QTG","dPHI_QQ","NJ20","NJ30","NJ_InGap","Alpha_B1H","Alpha_Q1QQ","DETA_BB_QQ","PT5","ZEP","PHIA_QQ_BB","MIN_R_B_Q","DR_Q1_BB","DR_Q2_BB","Norm_PT"
  //                      };

  if( Var_Name[variable] == "MINVQQ") _Var_VBF_Hbb->GetXaxis()->SetTitle("m_{qq} [GeV]");
  else if ( Var_Name[variable] == "DETAQQ") _Var_VBF_Hbb->GetXaxis()->SetTitle("|#Delta#eta_{qq}|");
  else if ( Var_Name[variable] == "dPHI_QQ") _Var_VBF_Hbb->GetXaxis()->SetTitle("|#Delta#phi_{qq}|");
  else if ( Var_Name[variable] == "BTG1_DeepJet") _Var_VBF_Hbb->GetXaxis()->SetTitle("Lead b-jet DEEPJET score");
  else if ( Var_Name[variable] == "BTG2_DeepJet") _Var_VBF_Hbb->GetXaxis()->SetTitle("Sublead b-jet DEEPJET score");
  else if ( Var_Name[variable] == "Q1_QTG") _Var_VBF_Hbb->GetXaxis()->SetTitle("Lead VBF-jet QCD score");
  else if ( Var_Name[variable] == "Q2_QTG") _Var_VBF_Hbb->GetXaxis()->SetTitle("Sublead VBF-jet QCD score");
  else if ( Var_Name[variable] == "NJ20") _Var_VBF_Hbb->GetXaxis()->SetTitle("Jet multiplicity for p_{T} > 20 GeV");
  else if ( Var_Name[variable] == "Alpha_Q1QQ") _Var_VBF_Hbb->GetXaxis()->SetTitle("#alpha_{qq}");
  else if ( Var_Name[variable] == "PHIA_QQ_BB") _Var_VBF_Hbb->GetXaxis()->SetTitle("#Delta (#phi_{qq} - #phi_{bb})");
  else if ( Var_Name[variable] == "DR_Q1_BB") _Var_VBF_Hbb->GetXaxis()->SetTitle("#Delta R_{H,q1}");
  else if ( Var_Name[variable] == "Norm_PT") _Var_VBF_Hbb->GetXaxis()->SetTitle("#sum #vec{p}^{i}_{T}/#sum p^{i}_{T}");
  else if ( Var_Name[variable] == "ZEP")  _Var_VBF_Hbb->GetXaxis()->SetTitle("#eta_{bb}- 0.5*(#eta_{q1} + #eta_{q2})");
  else if ( Var_Name[variable] == "pzAll")  _Var_VBF_Hbb->GetXaxis()->SetTitle("#sump_{Z} (b1 + b2 + q1 + q2) [GeV]");
  else if ( Var_Name[variable] == "PTb1")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Leading b-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PTb2")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Subleading b-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PTq1")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Leading q-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PTq2")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Subleading q-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PT1")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");	 
  else if ( Var_Name[variable] == "PT2")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Subleading jet p_{T} [GeV]");      
  else if ( Var_Name[variable] == "PT3")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Third jet p_{T} [GeV]");      
  else if ( Var_Name[variable] == "PT4")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Fourth jet p_{T} [GeV]");      
  else if ( Var_Name[variable] == "PTQ1")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Leading VBF parton [GeV]");
  else if ( Var_Name[variable] == "PTQ2")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Subleading VBF parton [GeV]");
  else if ( Var_Name[variable] == "PTB1")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Leading b parton [GeV]");
  else if ( Var_Name[variable] == "PTB2")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Subleading b parton [GeV]");
  else if ( Var_Name[variable] == "MQQ")  _Var_VBF_Hbb->GetXaxis()->SetTitle("parton level M_{QQ} [GeV]");
  else if ( Var_Name[variable] == "PTQ1")  _Var_VBF_Hbb->GetXaxis()->SetTitle("parton level M_{bb} [GeV]");
  else if ( Var_Name[variable] == "Njet_20")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Number of jets with p_{T} > 20 GeV");
  else if ( Var_Name[variable] == "Njet_30")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Number of jets with p_{T} > 30 GeV");
  else if ( Var_Name[variable] == "Njet_20_ctrl")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Number of jets with p_{T} > 20 GeV & |#eta| < 2.5");
  else if ( Var_Name[variable] == "Njet_30_ctrl")  _Var_VBF_Hbb->GetXaxis()->SetTitle("Number of jets with p_{T} > 30 GeV & |#eta| < 2.5");
  //Var
  else if ( Var_Name[variable] == "bdtout")  _Var_VBF_Hbb->GetXaxis()->SetTitle("BDT output");


   _Var_VBF_Hbb->GetYaxis()->SetTitle(xtitle);
   Float_t ymax;
   if ( _Var_VBF_Hbb->GetMaximum() <  _Var_VBF_Hbb_H->GetMaximum())  { ymax =  _Var_VBF_Hbb_H->GetMaximum(); }
   else { ymax = _Var_VBF_Hbb->GetMaximum();}

   gStyle->SetLineWidth(2);
   TCanvas *c = new TCanvas("c", "c",241,94,800,727);
   c->Range(-23.5781,-0.1522388,160.8859,1.35);
   c->SetFillColor(0);
   c->SetBorderMode(0);
   c->SetBorderSize(2);
   c->SetGridx();
   c->SetGridy();
   c->SetTickx(1);
   c->SetTicky(1);
   c->SetLeftMargin(0.1278196);
   c->SetRightMargin(0.03884712);
   c->SetBottomMargin(0.1232092);
   c->SetFrameLineWidth(2);
   c->SetFrameBorderMode(0);


      TPad *pad1 = new TPad("pad1", "pad1",0,0.3,1,1);
      pad1->Draw();
      pad1->Range(-148.318,2.314742,1071.865,8.438788);
      pad1->SetFillColor(0);
      pad1->SetBorderMode(0);
      pad1->SetBorderSize(2);
      //pad1->SetLogy();
      pad1->SetGridx();
      pad1->SetGridy();
      pad1->SetTickx(1);
      pad1->SetTicky(1);
      pad1->SetLeftMargin(0.1215539);
      pad1->SetRightMargin(0.05889724);
      pad1->SetBottomMargin(0.005321326);
      pad1->SetFrameBorderMode(0);
      pad1->SetFrameBorderMode(0);


      TPad *pad2 = new TPad("pad2", "newpad",0,0,1,0.3);
      pad2->Draw();
      pad2->Range(-148.318,-0.4344828,1071.865,1.009655);
      pad2->SetGridx();
      pad2->SetGridy();
      pad2->SetTickx(1);
      pad2->SetTicky(1);
      pad2->SetFillColor(0);
      pad2->SetBorderMode(0);
      pad2->SetBorderSize(2);
      pad2->SetLeftMargin(0.1215539);
      pad2->SetRightMargin(0.06015038);
      pad2->SetTopMargin(0.006685769);
      pad2->SetBottomMargin(0.3008596);
      pad2->SetFrameBorderMode(0);
      pad2->SetFrameBorderMode(0);




  pad1->cd();
  _Var_VBF_Hbb->SetTitle("");
  _Var_VBF_Hbb->SetStats(0);
  _Var_VBF_Hbb->SetLineWidth(2);
  _Var_VBF_Hbb->SetLineColor(kBlack);
  _Var_VBF_Hbb->Draw("HIST");
  _Var_VBF_Hbb->GetYaxis()->SetRangeUser(0.0 , ymax*1.2);

  _Var_VBF_Hbb_H->SetLineWidth(2);
  _Var_VBF_Hbb_H->SetFillColor(kCyan);
  _Var_VBF_Hbb_H->SetFillStyle(3325);
  _Var_VBF_Hbb_H->SetLineColor(kBlue);
  _Var_VBF_Hbb_H->Draw("HIST,SAME");



  TLegend *legend1 = new TLegend(0.65, 0.75, 0.90, 0.85);
  legend1->SetTextFont(42);
  legend1->SetLineColor(0);
  legend1->SetTextSize(0.03);
  legend1->SetFillColor(0);
  legend1->SetFillStyle(0);
  legend1->AddEntry(_Var_VBF_Hbb, "w/o k-factor", "l");
  legend1->AddEntry(_Var_VBF_Hbb_H, "with k-fact", "lf");
  legend1->Draw();

  

  TLatex *t2a = new TLatex(0.54,0.91," #bf{CMS} #it{Preliminary}                      54.4 fb^{-1}, (2018, 13 TeV)");
  t2a->SetNDC();
  t2a->SetTextFont(42);
  t2a->SetTextSize(0.04);
  t2a->SetTextAlign(20);
  t2a->Draw("same");
  
  TLatex * text_proc = new TLatex(0.19,0.83,"#font[61]{#splitline{VBFH#rightarrow b#bar{b}}{Double B-Tag}}");
  text_proc->SetNDC();
  text_proc->SetTextFont(32);
  text_proc->SetTextSize(0.030);
  text_proc->SetTextColor(kBlack);
  text_proc->Draw("same");

   c->cd();
   pad2->cd();
   TH1F *h_nm2 = (TH1F*)  _Var_VBF_Hbb->Clone();
   TH1F *h_up2 = (TH1F*)  _Var_VBF_Hbb_H->Clone();

   TH1F *h_nm3 = (TH1F*)  _Var_VBF_Hbb->Clone();
   TH1F *h_up3 = (TH1F*)  _Var_VBF_Hbb_H->Clone();

   //h_up2->Add(h_nm2,-1);

   h_up2->Divide(h_nm2);

   TH1F *h_up5 = (TH1F*)  h_up2->Clone();

   //h_up5->Scale(-1);
   //h_dn5->Scale(-1);


   h_up2->SetStats(0);
   h_up2->SetTitle("");
   h_up2->SetMarkerColor(kBlue+1);
   h_up2->SetLineColor(kBlue+1);
   h_up2->GetXaxis()->SetTitle("Double-B analysis category");
  if( Var_Name[variable] == "MINVQQ") h_up2->GetXaxis()->SetTitle("m_{qq} [GeV]");
  else if ( Var_Name[variable] == "DETAQQ") h_up2->GetXaxis()->SetTitle("|#Delta#eta_{qq}|");
  else if ( Var_Name[variable] == "dPHI_QQ") h_up2->GetXaxis()->SetTitle("|#Delta#phi_{qq}|");
  else if ( Var_Name[variable] == "BTG1_DeepJet") h_up2->GetXaxis()->SetTitle("Lead b-jet DEEPJET score");
  else if ( Var_Name[variable] == "BTG2_DeepJet") h_up2->GetXaxis()->SetTitle("Sublead b-jet DEEPJET score");
  else if ( Var_Name[variable] == "Q1_QTG") h_up2->GetXaxis()->SetTitle("Lead VBF-jet QCD score");
  else if ( Var_Name[variable] == "Q2_QTG") h_up2->GetXaxis()->SetTitle("Sublead VBF-jet QCD score");
  else if ( Var_Name[variable] == "NJ20") h_up2->GetXaxis()->SetTitle("Jet multiplicity for p_{T} > 20 GeV");
  else if ( Var_Name[variable] == "Alpha_Q1QQ") h_up2->GetXaxis()->SetTitle("#alpha_{qq}");
  else if ( Var_Name[variable] == "PHIA_QQ_BB") h_up2->GetXaxis()->SetTitle("#Delta (#phi_{qq} - #phi_{bb})");
  else if ( Var_Name[variable] == "DR_Q1_BB") h_up2->GetXaxis()->SetTitle("#Delta R_{H,q1}");
  else if ( Var_Name[variable] == "Norm_PT") h_up2->GetXaxis()->SetTitle("#sum #vec{p}^{i}_{T}/#sum p^{i}_{T}");
  else if ( Var_Name[variable] == "ZEP")  h_up2->GetXaxis()->SetTitle("#eta_{bb}- 0.5*(#eta_{q1} + #eta_{q2})");
  else if ( Var_Name[variable] == "pzAll")  h_up2->GetXaxis()->SetTitle("#sump_{Z} (b1 + b2 + q1 + q2) [GeV]");
  else if ( Var_Name[variable] == "PTb1")  h_up2->GetXaxis()->SetTitle("Leading b-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PTb2")  h_up2->GetXaxis()->SetTitle("Subleading b-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PTq1")  h_up2->GetXaxis()->SetTitle("Leading q-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PTq2")  h_up2->GetXaxis()->SetTitle("Subleading q-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PT1")  h_up2->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PT2")  h_up2->GetXaxis()->SetTitle("Subleading jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PT3")  h_up2->GetXaxis()->SetTitle("Third jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "PT4")  h_up2->GetXaxis()->SetTitle("Fourth jet p_{T} [GeV]");

  else if ( Var_Name[variable] == "ETA1")  h_up2->GetXaxis()->SetTitle("Leading jet #eta");
  else if ( Var_Name[variable] == "ETA2")  h_up2->GetXaxis()->SetTitle("Subleading jet #eta");
  else if ( Var_Name[variable] == "ETA3")  h_up2->GetXaxis()->SetTitle("Third jet #eta");
  else if ( Var_Name[variable] == "ETA4")  h_up2->GetXaxis()->SetTitle("Fourth jet #eta");

  else if ( Var_Name[variable] == "PTQ1")  h_up2->GetXaxis()->SetTitle("Leading VBF parton p_{T} [GeV]");
  else if ( Var_Name[variable] == "PTQ2")  h_up2->GetXaxis()->SetTitle("Subleading VBF parton p_{T} [GeV]");
  else if ( Var_Name[variable] == "PTB1")  h_up2->GetXaxis()->SetTitle("Leading b parton p_{T} [GeV]");
  else if ( Var_Name[variable] == "PTB2")  h_up2->GetXaxis()->SetTitle("Subleading b parton p_{T} [GeV]");
  else if ( Var_Name[variable] == "MQQ")   h_up2->GetXaxis()->SetTitle("parton level M_{QQ} [GeV]");
  else if ( Var_Name[variable] == "PTQ1")  h_up2->GetXaxis()->SetTitle("parton level M_{bb} [GeV]");
  //matched gen 
  else if ( Var_Name[variable] == "ptQ1")  h_up2->GetXaxis()->SetTitle("Leading VBF parton matched gen jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "ptQ2")  h_up2->GetXaxis()->SetTitle("Subleading VBF parton matched gen jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "ptB1")  h_up2->GetXaxis()->SetTitle("Leading b parton matched gen jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "ptB2")  h_up2->GetXaxis()->SetTitle("Subleading b parton matched gen jet p_{T} [GeV]");
  //gen 
  else if ( Var_Name[variable] == "pt1")  h_up2->GetXaxis()->SetTitle("Leading gen-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "pt2")  h_up2->GetXaxis()->SetTitle("Subleading gen-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "pt3")  h_up2->GetXaxis()->SetTitle("Third gen-jet p_{T} [GeV]");
  else if ( Var_Name[variable] == "pt4")  h_up2->GetXaxis()->SetTitle("Fourth gen-jet p_{T} [GeV]");
  //Njets
  else if ( Var_Name[variable] == "Njet_20")  h_up2->GetXaxis()->SetTitle("Number of jets with p_{T} > 20 GeV");
  else if ( Var_Name[variable] == "Njet_30")  h_up2->GetXaxis()->SetTitle("Number of jets with p_{T} > 30 GeV");
  else if ( Var_Name[variable] == "Njet_20_ctrl")  h_up2->GetXaxis()->SetTitle("Number of jets with p_{T} > 20 GeV & |#eta| < 2.5");
  else if ( Var_Name[variable] == "Njet_30_ctrl")  h_up2->GetXaxis()->SetTitle("Number of jets with p_{T} > 30 GeV & |#eta| < 2.5");

   h_up2->GetXaxis()->SetNdivisions(403);
   h_up2->GetXaxis()->SetRange(1,182);
   h_up2->GetXaxis()->SetLabelFont(42);
   h_up2->GetXaxis()->SetLabelSize(0.09);
   h_up2->GetXaxis()->SetTitleSize(0.11);
   h_up2->GetXaxis()->SetTickLength(0.08);
   h_up2->GetXaxis()->SetTitleOffset(1.05);
   h_up2->GetXaxis()->SetTitleFont(42);
   h_up2->GetYaxis()->SetTitle("#frac{with k-fact}{w/o k-fact}");
   h_up2->GetYaxis()->SetNdivisions(207);
   h_up2->GetYaxis()->SetLabelFont(42);
   h_up2->GetYaxis()->SetLabelSize(0.09);
   h_up2->GetYaxis()->SetTitleSize(0.11);
   h_up2->GetYaxis()->SetTickLength(0.02);
   h_up2->GetYaxis()->SetTitleOffset(0.42);
   h_up2->GetYaxis()->SetTitleFont(42);
   h_up2->GetYaxis()->SetRangeUser(0.0,2.0);

   h_up2->GetXaxis()->SetRangeUser(Xmin[variable],Xmax[variable]);




   h_up2->SetLineColor(kBlue);
   h_up2->Draw("HIST");

   //h_up5->Draw("HIST,SAME");
   //h_up5->SetLineColor(kRed);

   //h_dn5->Draw("HIST,SAME");
   //h_dn5->SetLineColor(kBlue);

   //h_dn2->SetLineColor(kRed);
   //h_dn2->Draw("HIST,SAME");

   TLegend *legend_up = new TLegend(0.15, 0.80, 0.25, 0.88);
   legend_up->SetTextFont(42);
   legend_up->SetLineColor(0);
   legend_up->SetTextSize(0.080);
   legend_up->SetFillStyle(0);
   legend_up->AddEntry(h_up2, "( with  - w/o k-factors) ", "F");
   //legend_up->Draw("SAME");

   TLine *ln = new TLine(Xmin[variable],1.0,Xmax[variable],1.0);
   ln->SetLineColor(kRed);
   ln->Draw("same");


  if(Var_Name[variable] == "ZEP" ||  Var_Name[variable] == "MINVQQ" || Var_Name[variable] == "DETAQQ" || Var_Name[variable] == "dPHI_QQ" || Var_Name[variable] == "BTG1_DeepJet" || Var_Name[variable] == "BTG2_DeepJet" || Var_Name[variable] == "Q1_QTG" ||  Var_Name[variable] == "Q2_QTG" || Var_Name[variable] == "pzAll" || Var_Name[variable] == "NJ20" ||  Var_Name[variable] == "Alpha_Q1QQ" || Var_Name[variable] == "PHIA_QQ_BB" || Var_Name[variable] == "DR_Q1_BB" || Var_Name[variable] == "Norm_PT" || Var_Name[variable] == "E_rest_20" || Var_Name[variable] == "HTT_rest_20" ||  Var_Name[variable] == "PTb1" ||  Var_Name[variable] =="PTb2" ||  Var_Name[variable] =="PTq1" ||  Var_Name[variable] =="PTq2" || Var_Name[variable] ==   "PT1" || Var_Name[variable] ==  "PT2" || Var_Name[variable] ==  "PT3" || Var_Name[variable] ==  "PT4" || Var_Name[variable] == "PTQ1" || Var_Name[variable] == "PTQ2" || Var_Name[variable] == "PTB1" || Var_Name[variable] == "PTB2" || Var_Name[variable] == "MBB" || Var_Name[variable] == "MQQ" || Var_Name[variable] == "ptQ1" || Var_Name[variable] == "ptQ2" || Var_Name[variable] == "ptB1" || Var_Name[variable] == "ptB2" || Var_Name[variable] == "pt1" || Var_Name[variable] == "pt2" || Var_Name[variable] == "pt3" || Var_Name[variable] == "pt4" || Var_Name[variable] ==   "ETA1" || Var_Name[variable] ==  "ETA2" || Var_Name[variable] ==  "ETA3" || Var_Name[variable] ==  "ETA4" || Var_Name[variable] == "Njet_20" || Var_Name[variable] == "Njet_30" || Var_Name[variable] == "Njet_20_ctrl" || Var_Name[variable] == "Njet_30_ctrl" || Var_Name[variable] == "bdtout" )
    {
	  c->Print("Plots/png/COM_BDT_2018/Non_Log_"+Var_Name[variable]+"_DoubleB_2018.png");
     	  c->Print("Plots/pdf/COM_BDT_2018/Non_Log_"+Var_Name[variable]+"_DoubleB_2018.pdf");  
    } 
   }
}

