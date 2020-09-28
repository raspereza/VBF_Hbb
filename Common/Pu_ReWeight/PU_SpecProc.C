#include "TStyle.h"
#include "TGaxis.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <math.h>
#include <TF1.h>
#include <TF2.h>
#include <TH1D.h>
#include "TCanvas.h"
#include "TROOT.h"
#include "TNtuple.h"


void PU_SpecProc()
{

  TFile *f = new TFile("../../SingleBTag/Preselected_NTuples/Training__VBFHToBB_M_125__.root");
  TTree *other = (TTree*)f->Get("Training/Other_variables");
  Float_t npv;
  other->SetBranchAddress("Tnpv",&npv);

  TFile *f1 = new TFile("PUSPEC.root","RECREATE");
  TH1D *_PUMC = new TH1D("PUMC","PUMC",100,0,100);

  for(int i=0;i<other->GetEntries();i++)
   {
    other->GetEntry(i);
    _PUMC->Fill(npv);
   }
f1->Write();
}


