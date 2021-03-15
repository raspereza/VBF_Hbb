#include "HttStylesNew.cc"
#include "Common.h"

void PlotZbbDSCB(int iCAT=-1) {

  using namespace RooFit;

  TH1::SetDefaultSumw2(true);
  
  SetStyle();

  double B0 = 0.5;
  double B1 = 0.5;
  double B2 = 0.5;
  double FSig = 0.5;

  std::map<TString, TNtuple*> tree;
  for (int i=0; i<5; ++i) {
    TFile * file = new TFile(dirName+"/"+FileNamesBDT[i]);
    tree[sysNames[i]] = (TNtuple*)file->Get("Mass_and_BDT_ZJets");
  }
  
  std::map<TString, double> mapMean;
  std::map<TString, double> mapSigma;
  std::map<TString, double> mapAlpha;
  std::map<TString, double> mapN;
  std::map<TString, double> mapFSig;
  std::map<TString, double> mapNorm;

  for (auto sysName : sysNames) {
    
    TNtuple * treezbb = tree[sysName];

    TString namezbbhist = "mbb_zbb";

    TH1D * histzbb = new TH1D(namezbbhist,"",NbinsSig,xmin,xmax);

    TCanvas * dummy = new TCanvas("dummy","",800,700);
    if (iCAT>=0)
      treezbb->Draw("mbb>>"+namezbbhist,"weight*("+cuts[iCAT]+")");
    else
      treezbb->Draw("mbb>>"+namezbbhist,"weight");

    delete dummy;

    InitData(histzbb);
    InitHist(histzbb,"","",kGreen,1001);

    RooRealVar mbb("mbb","mass(bb)",xmin,xmax);

    RooRealVar mean("mean","Mean",92,80,200);
    RooRealVar sigma("sigma","Width",10,0,30);
    RooRealVar alpha("alpha","Alpha",2,0,100);
    RooRealVar n("n","n",1,0,100);
    
    RooRealVar b0("b0","b0",0.5,0,1);
    RooRealVar b1("b1","b1",0.5,0,1);
    RooRealVar b2("b2","b2",0.5,0,1);
    RooRealVar fsig("fsig","fsig",0.5,0,1);

    if (!sysName.Contains("Nom")) {
      b0.setVal(B0);
      b1.setVal(B1);
      b2.setVal(B2);
      fsig.setVal(FSig);
      b0.setConstant(kTRUE);
      b1.setConstant(kTRUE);
      b2.setConstant(kTRUE);
      fsig.setConstant(kTRUE);
    }
    

    RooBernstein BRN("brn","Bernstein",mbb,RooArgList(b0,b1,b2));
    RooCBShape cb("cb","CBshape",mbb,mean,sigma,alpha,n);
    RooGaussian gaus("gaus","gauss",mbb,mean,sigma);
    RooAddPdf signal("signal","signal",RooArgList(gaus,BRN),fsig);
    
    RooDataHist data("data","data",mbb,histzbb);

    signal.fitTo(data,Save(),SumW2Error(kTRUE));
    RooPlot * frame = mbb.frame();
    data.plotOn(frame);
    signal.plotOn(frame,Name("signal"));
    signal.plotOn(frame,Components(BRN),LineStyle(kDashed));

    mapMean[sysName] = mean.getValV();
    mapSigma[sysName] = sigma.getValV();
    mapAlpha[sysName] = alpha.getValV();
    mapN[sysName] = n.getValV();
    mapFSig[sysName] = fsig.getValV();
    mapNorm[sysName] = histzbb->GetSumOfWeights();

    if (sysName.Contains("Nom")) {
      B0 = b0.getValV();
      B1 = b1.getValV();
      B2 = b2.getValV();
      FSig = fsig.getValV();
    }
    TH1D * linezbb = new TH1D("linezbb","",2,0,1);
    linezbb->SetLineColor(kBlue);
    linezbb->SetLineWidth(3);

    frame->SetTitle("");
    frame->GetYaxis()->SetTitle("Events / 5 GeV");
    frame->GetXaxis()->SetTitle("m_{bb} [GeV]");
    frame->GetYaxis()->SetTitleOffset(1.5);
    frame->GetYaxis()->SetRangeUser(0.,1.2*histzbb->GetMaximum()); 
    TCanvas * canv = new TCanvas("canv","",700,700);
    frame->Draw();
    histzbb->Draw("hsame"); 
    frame->Draw("same");
    TLegend * leg = new TLegend(0.6,0.65,0.8,0.8);
    SetLegendStyle(leg);
    leg->AddEntry(linezbb,"Fit", "l");
    leg->AddEntry(histzbb,"Zbb","F");
    leg->Draw();
    canv->Update();
    canv->Print("ZJets_"+sysName+".png");
    delete canv;
  }

  std::cout << "           Mean  Sigma  Fsig   Norm" << std::endl;
  //            Nominal :  92.6  11.18  0.66  1312.8 
  for (auto sysName : sysNames) {
    std::cout << label[sysName] << " : ";
    printf("%5.1f  %5.2f  %4.2f  %6.1f\n",
	   mapMean[sysName],mapSigma[sysName],
	   mapFSig[sysName],mapNorm[sysName]);
  }

}
