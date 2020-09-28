#include "TStyle.h"
#include "TGaxis.h"
#include "TRandom.h"

#include <iostream>
#include <math.h>
#include <TF1.h>
#include <TH1D.h>
#include "TCanvas.h"

double rew[]={0.798126,18.1207,14.7655,8.96119,2.78256,2.39094,1.80281,1.18334,0.866385,4.50645,5.11604,4.87425,3.63377,2.3087,1.48188,1.03325,0.838923,0.773768,0.741412,0.698234,0.670365,0.687718,0.725647,0.740976,0.720316,0.684778,0.656715,0.639069,0.638062,0.63056,0.628136,0.624424,0.625472,0.628517,0.640495,0.668228,0.711752,0.768685,0.811498,0.860919,0.969558,1.12861,1.35588,1.66454,2.05542,2.50623,2.96154,3.21858,3.36422,3.26354,3.01841,2.63201,2.18626,1.7381,1.3301,0.963607,0.681615,0.465635,0.319755,0.220719,0.153924,0.108758,0.0779425,0.0566201,0.0416081,0.0263959,0.016734,0.0121966,0.00889823,0.00647328,0.00467944,0.00335151,0.00237284,0.00165781,0.000908819,0.000508386,0.000337097,0.000219905,0.000141031,8.88541e-05,5.49556e-05,3.33457e-05,1.98403e-05,9.02122e-06,4.21836e-06,2.36058e-06,1.29579e-06,6.98202e-07,3.69623e-07,1.92468e-07,9.87022e-08,4.99156e-08,1.91257e-08,7.61235e-09,3.69361e-09,1.77501e-09,8.43189e-10,3.99759e-10,1.83632e-10,0};
// For VBF as the MC-PU is old also
//double rew2[]={0.00152993,0.0208702,0.0120212,0.0231849,0.0640504,0.097894,0.11668,0.0889428,0.136214,0.247326,0.412895,0.467375,0.641918,0.641798,0.716324,0.718497,0.872162,0.84305,0.959629,0.928358,1.01642,1.00977,1.07727,1.02761,1.06245,1.04113,1.0884,1.11808,1.07781,1.09916,1.12324,1.07555,1.09423,1.12447,1.156,1.14111,1.07118,1.12979,1.10759,1.11957,1.16809,1.13577,1.22181,1.09211,1.25752,1.1989,1.21682,1.17944,1.11435,1.12213,1.14098,1.20118,1.08931,1.12374,1.25526,1.08478,1.16517,1.17182,1.23103,1.21718,1.29821,1.27825,1.25741,1.15848,1.45292,1.26303,1.8664,1.17183,1.20768,1.37957,0.987194,1.86301,0.980219,1.36579,1.78276,1.24187,2.17514,0.163849,0.172894,1.788,0.287794,0.0434594,0.472933,1,1,0.0385193,1,1,1,0.0103688,1,1,1,1,1,1,1,1,1,1};
void Compare()
{
//  for(int i=0;i<100;i++)
//    rew[i]=1;
  gStyle->SetOptFile(0);
  gStyle->SetOptStat(0);

  TFile* f1 = new TFile("PileupMC.root");
  TH1D * histo1 = (TH1D*)f1->Get("input_Event/N_TrueInteractions");
  TFile* f2 = new TFile("MyDataPileupHistogram.root");
  TH1D * histo2 = (TH1D*)f2->Get("pileup");
//  TFile* f1 = new TFile("PUSPEC.root");
//  TH1D * histo1 = (TH1D*)f1->Get("PUMC");

  cout<<histo1->GetSumOfWeights()<<endl;
  cout<<histo2->GetSumOfWeights()<<endl;

  TH1D * _NhistoM = new TH1D("NhistoM","NhistoM",100,0,100);
  TH1D * _NhistoD = new TH1D("NhistoD","NhistoD",100,0,100);

  for(int i=1;i<=100;i++)
   {
    if(i<=-10 || i>=600)
    {
     _NhistoM->SetBinContent(i,0);
     _NhistoD->SetBinContent(i,0);
    }
    else
    {
//     _NhistoM->SetBinContent(i,histo1->GetBinContent(i)*rew[i-1]*rew2[i-1]);  // For VBF only
//     _NhistoM->SetBinContent(i,histo1->GetBinContent(i)*rew[i-1]);
     _NhistoM->SetBinContent(i,histo1->GetBinContent(i));
     _NhistoD->SetBinContent(i,histo2->GetBinContent(i));
    }
   }

  _NhistoM->Scale(1/_NhistoM->GetSumOfWeights());
  _NhistoD->Scale(1/_NhistoD->GetSumOfWeights());

  _NhistoD->Draw("HIST same");
  _NhistoM->Draw("HIST same");

  for(int i=1;i<=100;i++)
//    cout<<histo2->GetBinContent(i+1)/histo1->GetBinContent(i+1)<<",";
    cout<<_NhistoD->GetBinContent(i)/_NhistoM->GetBinContent(i)<<",";
  cout<<endl;

}