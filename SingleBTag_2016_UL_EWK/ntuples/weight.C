#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCut.h"
#include "TPaveText.h"
#include "TGaxis.h"
#include "TProfile.h"
#include "TCut.h"
#include <iostream>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
using namespace std;
void weight()
{

TString proc_Name[] ={"Mass_and_BDT_VBF_Hbb_Dipol","Mass_and_BDT_ggF_Hbb_POWHEG","Mass_and_BDT_QCD","Mass_and_BDT_Sandgle_Top","Mass_and_BDT_tt","Mass_and_BDT_WJets","Mass_and_BDT_ZJets","Mass_and_BDT_ZJets_EWK","Mass_and_BDT_DATA"};
TString proc_name;

 std::cout << " Event yeilds table for 2018 " << std::endl;
 float mbb_max = 200.0;
 float mbb_min = 80.0;
 bool catwise = false;
 if (mbb_min > 0.0  ) {std::cout << "Inside the mass range of " << mbb_min << "	< mbb_reg < " <<  mbb_max << std::endl;}
 int nproc = sizeof(proc_Name)/sizeof(proc_Name[0]);
 if(catwise) {std::cout << "Proc	&&	CAT0[Rej]	&&	CAT1	 &&	CAT2	&&	CAT3 // " << std::endl;}
 for (int ii=0;ii<nproc;ii++)
  {
   TString proc_name;
   if(ii == 0 ) {proc_name = "VBFHbb";}
   else if(ii == 1 ) {proc_name = "GGFHbb(PHG)";}
   else if(ii == 2 ) {proc_name = "QCD";}
   else if(ii == 3 ) {proc_name = "ST";}
   else if(ii == 4 ) {proc_name = "tt";}
   else if(ii == 5 ) {proc_name = "WJets";}
   else if(ii == 6 ) {proc_name = "ZJets";}
   else if(ii == 7 ) {proc_name = "ZJets_EWK";}
   else if(ii == 8 ) {proc_name = "DATA";}

   //std::cout << "Proc : " << proc_Name[ii] << std::endl;
   TFile *file = TFile::Open("mbb_and_bdt_all_JER_up.root");
   TTree *tree = (TTree*)file->Get(proc_Name[ii]);
   Float_t wt; Float_t bdt, mbb_reg;
   tree->SetBranchAddress("weight", &wt);
   tree->SetBranchAddress("bdtout", &bdt);
   tree->SetBranchAddress("mbb_reg", &mbb_reg);
   //Float_t wt0=0.0, wt1=0.0, wt2=0.0, wt3=0.0, wt4=0.0, wt5 = 0.0, wt6 = 0.0, wt7 = 0.0;
   Float_t tot_wt = 0.0;
   Float_t wt0=0.0, wt1=0.0, wt2=0.0, wt3=0.0;
   Long64_t nn = tree->GetEntries();
   for(Long64_t j =0; j < nn ; j++)
    {
     tree->GetEntry(j);
     if( mbb_reg < mbb_min || mbb_reg > mbb_max) continue;
     if (proc_Name[ii] == "Mass_and_BDT_DATA" && ( mbb_reg > 100 && mbb_reg < 150)) continue;
     tot_wt = tot_wt + wt; 
     if(bdt < 0.25) wt0 = wt0 + wt;
     else if (bdt > 0.25 && bdt < 0.5) wt1 = wt1 + wt;
     else if (bdt > 0.50 && bdt < 0.75) wt2 = wt2 + wt;
     else if (bdt > 0.75) wt3 = wt3 + wt;
    }
   if(catwise) {std::cout << fixed << setprecision(2) << proc_name << " & " << wt0 << " & " << wt1 << " & " << wt2 << " & " << wt3 <<  
	  std::endl;}
   else {std::cout << proc_Name[ii] << "	&&	" << tot_wt << "  &&   " << sqrt(tot_wt) <<     std::endl;}
  }
 }
