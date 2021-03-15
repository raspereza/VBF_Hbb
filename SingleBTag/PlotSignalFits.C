#include "HttStylesNew.cc"
#include "Common.h"

void PlotSignalFits(int iCAT=-1) {

  using namespace RooFit;

  TH1::SetDefaultSumw2(true);
  
  SetStyle();

  std::map<TString, TNtuple*> treeGGH;
  std::map<TString, TNtuple*> treeVBF;
  for (int i=0; i<5; ++i) {
    TFile * file = new TFile(dirName+"/"+FileNamesBDT[i]);
    treeGGH[sysNames[i]] = (TNtuple*)file->Get("Mass_and_BDT_ggF_Hbb");
    treeVBF[sysNames[i]] = (TNtuple*)file->Get("Mass_and_BDT_VBF_Hbb");
  }
  
  std::map<TString, double> mapMean;
  std::map<TString, double> mapSigma;
  std::map<TString, double> mapAlpha;
  std::map<TString, double> mapN;
  std::map<TString, double> mapFSig;
  std::map<TString, double> mapNormGGH;
  std::map<TString, double> mapNormVBF;

  double B0 = 0.5;
  double B1 = 0.5;
  double B2 = 0.5;
  double Alpha_CB = 2;
  double N_CB = 2;
  double FSig = 0.5;

  for (auto sysName : sysNames) {
    
    TString nameGGHHist = "mbb_ggh";
    TString nameVBFHist = "mbb_vbf";
    TH1D * histGGH = new TH1D(nameGGHHist,"",NbinsSig,xmin,xmax);
    TH1D * histVBF = new TH1D(nameVBFHist,"",NbinsSig,xmin,xmax);
    TCanvas * dummy = new TCanvas("dummy","",800,700);
    if (iCAT>=0) {
      treeGGH[sysName]->Draw("mbb>>"+nameGGHHist,"weight*("+cuts[iCAT]+")");
      treeVBF[sysName]->Draw("mbb>>"+nameVBFHist,"weight*("+cuts[iCAT]+")");
    }
    else {
      treeGGH[sysName]->Draw("mbb>>"+nameGGHHist,"weight");
      treeVBF[sysName]->Draw("mbb>>"+nameVBFHist,"weight");
    }
    mapNormVBF[sysName] = histVBF->GetSumOfWeights();
    mapNormGGH[sysName] = histGGH->GetSumOfWeights();
    histVBF->Add(histGGH,histVBF);
    delete dummy;

    InitData(histVBF);
    InitHist(histGGH,"","",kCyan,1001);
    for (int iB=1; iB<120; ++iB)
      histGGH->SetBinError(iB,0);

    RooRealVar mbb("mbb","mass(bb)",xmin,xmax);
    RooRealVar mean("mean","Mean",125,80,200);
    RooRealVar sigma("sigma","Width",10,0,30);
    RooRealVar alpha("alpha","Alpha",2,0,100);
    RooRealVar n("n","n",2,0,100);
    
    RooRealVar b0("b0","b0",0.5,0,1);
    RooRealVar b1("b1","b1",0.5,0,1);
    RooRealVar b2("b2","b2",0.5,0,1);
    RooRealVar fsig("fsig","fsig",0.5,0,1);

    if (!sysName.Contains("Nom")) {
      b0.setVal(B0);
      b1.setVal(B1);
      b2.setVal(B2);
      alpha.setVal(Alpha_CB);
      n.setVal(N_CB);
      fsig.setVal(FSig);
      b0.setConstant(kTRUE);
      b1.setConstant(kTRUE);
      b2.setConstant(kTRUE);
      alpha.setConstant(kTRUE);
      n.setConstant(kTRUE);
      fsig.setConstant(kTRUE);
    }

    RooBernstein BRN("brn","Bernstein",mbb,RooArgList(b0,b1,b2));
    RooCBShape cb("cb","CBshape",mbb,mean,sigma,alpha,n);
    RooGaussian gaus("gaus","Gauss",mbb,mean,sigma); 
    RooAddPdf signal("signal","signal",RooArgList(cb,BRN),fsig);
    RooDataHist data("data","data",mbb,histVBF);

    signal.fitTo(data,Save(),SumW2Error(kTRUE));
    RooPlot * frame = mbb.frame();
    data.plotOn(frame,Name("data"));
    signal.plotOn(frame,Name("signal"));
    signal.plotOn(frame,Components(BRN),LineStyle(kDashed));

    mapMean[sysName] = mean.getValV();
    mapSigma[sysName] = sigma.getValV();
    mapAlpha[sysName] = alpha.getValV();
    mapN[sysName] = n.getValV();
    mapFSig[sysName] = fsig.getValV();

    if (sysName.Contains("Nom")) {
      B0 = b0.getValV();
      B1 = b1.getValV();
      B2 = b2.getValV();
      Alpha_CB = alpha.getValV();
      N_CB = n.getValV();
      FSig = fsig.getValV();
    }

    frame->SetTitle("              ");
    frame->GetYaxis()->SetTitle("Events / 5 GeV");
    frame->GetXaxis()->SetTitle("m_{bb} [GeV]");
    frame->GetYaxis()->SetTitleOffset(1.5);
    frame->GetYaxis()->SetRangeUser(0.,1.2*histVBF->GetMaximum());
    
    TCanvas * canv = new TCanvas("canv","",700,700);
    frame->Draw();
    histGGH->Draw("hsame");
    frame->Draw("same");
    TLegend * leg = new TLegend(0.6,0.65,0.8,0.8);
    SetLegendStyle(leg);
    leg->AddEntry(histVBF,"ggH+qqH","lp");
    leg->AddEntry(histGGH,"ggH","f");
    leg->Draw();
    canv->Update();
    canv->Print("signal_"+sysName+".png");
  }

  std::cout << std::endl;
  std::cout << "           Mean  Sigma  Alpha    N     FSig  N_qqH  N_ggH" << std::endl;
  //            Nominal : 121.2  14.32   5.31  0.0005  0.92  443.2  394.0

  for (auto sysName : sysNames) {
    std::cout << label[sysName] << " : ";
    printf("%5.1f  %5.2f  %5.2f  %6.4f  %4.2f  %5.1f  %5.1f\n",
	   mapMean[sysName],mapSigma[sysName],
	   mapAlpha[sysName],mapN[sysName],mapFSig[sysName],
	   mapNormVBF[sysName],mapNormGGH[sysName]);
  }

}
