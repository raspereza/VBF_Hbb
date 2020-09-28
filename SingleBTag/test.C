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

void test()
{
   TFile * f  = new TFile("mbb_and_BDT.root");
   TTree * _Sig0 = (TTree*)f->Get("MinvS0");
   TTree * _Sig1 = (TTree*)f->Get("MinvS1");
   TTree * _Bkgr = (TTree*)f->Get("MinvB");
   TTree * _Obsv = (TTree*)f->Get("MinvO");

   Float_t weight_S0,mbb_S0,bdtout_S0;
   Float_t weight_S1,mbb_S1,bdtout_S1;
   Float_t weight_B,mbb_B,bdtout_B;
   Float_t weight_O,mbb_O,bdtout_O;
   _Sig0->SetBranchAddress( "weight", &weight_S0);
   _Sig0->SetBranchAddress( "mbb",    &mbb_S0);
   _Sig0->SetBranchAddress( "bdtout", &bdtout_S0);
   _Sig1->SetBranchAddress( "weight", &weight_S1);
   _Sig1->SetBranchAddress( "mbb",    &mbb_S1);
   _Sig1->SetBranchAddress( "bdtout", &bdtout_S1);
   _Bkgr->SetBranchAddress( "weight", &weight_B);
   _Bkgr->SetBranchAddress( "mbb",    &mbb_B);
   _Bkgr->SetBranchAddress( "bdtout", &bdtout_B);
   _Obsv->SetBranchAddress( "weight", &weight_O);
   _Obsv->SetBranchAddress( "mbb",    &mbb_O);
   _Obsv->SetBranchAddress( "bdtout", &bdtout_O);

//***************************************************************************************
  TFile *f1 = new TFile("res.root","RECREATE");
  gStyle->SetOptFile(0);
  gStyle->SetOptStat("mre");
  gStyle->SetPaintTextFormat("1.2e");
  TH1D * _Minv_S0  = new TH1D("Minv_S0","Minv_S0",100,0,500);
  TH1D * _Minv_S1  = new TH1D("Minv_S1","Minv_S1",100,0,500);
  TH1D * _Minv_B   = new TH1D("Minv_B","Minv_B",100,0,500);
  TH1D * _Minv_O   = new TH1D("Minv_O","Minv_O",100,0,500);

//***************************************************************************************
   double BDT_Cut_Max= 1.00;
   double BDT_Cut_Min= 0.85;

   for(int i=0;i<_Sig0->GetEntries();i++)
     {
      _Sig0->GetEntry(i);
      if(bdtout_S0>=BDT_Cut_Min && bdtout_S0<BDT_Cut_Max)
       _Minv_S0->Fill(mbb_S0,weight_S0);
     }

   for(int i=0;i<_Sig1->GetEntries();i++)
     {
      _Sig1->GetEntry(i);
      if(bdtout_S1>=BDT_Cut_Min && bdtout_S1<BDT_Cut_Max)
       _Minv_S1->Fill(mbb_S1,weight_S1);
     }

   for(int i=0;i<_Bkgr->GetEntries();i++)
     {
      _Bkgr->GetEntry(i);
      if(bdtout_B>=BDT_Cut_Min && bdtout_B<BDT_Cut_Max)
       _Minv_B->Fill(mbb_B,weight_B);
     }

   for(int i=0;i<_Obsv->GetEntries();i++)
     {
      _Obsv->GetEntry(i);
      if(bdtout_O>=BDT_Cut_Min && bdtout_O<BDT_Cut_Max)
       _Minv_O->Fill(mbb_O,weight_O);
     }
  f1->Write();

//***************************************************************************************

  TCanvas *Convas_1 = new TCanvas();
  double ymin=10e-2;
  double ymax=2*(_Minv_O->GetMaximum());

  _Minv_O->Draw("HIST same");
//  _Minv_O->SetFillColor(43);
  _Minv_O->SetLineWidth(2);
  _Minv_O->SetLineColor(2);
  _Minv_O->SetMaximum(ymax);
  _Minv_O->SetMinimum(ymin);

//  _Minv_B->Draw("HIST same");
//  _Minv_B->SetFillColor(43);
  _Minv_B->SetLineWidth(2);
  _Minv_B->SetLineColor(3);
  _Minv_B->SetMaximum(ymax);
  _Minv_B->SetMinimum(ymin);


//  _Minv_S1->Draw("HIST same");
//  _Minv_S1->SetFillColor(43);
  _Minv_S1->SetLineWidth(2);
  _Minv_S1->SetLineColor(5);
  _Minv_S1->SetMaximum(ymax);
  _Minv_S1->SetMinimum(ymin);


   _Minv_S0->Add(_Minv_S1);
  _Minv_S0->Draw("HIST same");
//  _Minv_S0->SetFillColor(43);
  _Minv_S0->SetLineWidth(2);
  _Minv_S0->SetLineColor(4);
  _Minv_S0->SetMaximum(ymax);
  _Minv_S0->SetMinimum(ymin);

}

