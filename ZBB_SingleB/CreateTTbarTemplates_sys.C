#include "Common.h"

using namespace RooFit;

std::ofstream outtext("tt_out.txt");

void CreateSignalPDF(int iCAT,
		     std::map<TString, TNtuple*> treeTTbar,
		     RooRealVar theta_JES,
		     RooRealVar theta_JER,
		     RooWorkspace * w) {

  TH1::SetDefaultSumw2(true);

  std::map<TString,double> mapMean;
  std::map<TString,double> mapSigma;
  std::map<TString,double> mapNorm;

  TCanvas * dummy = new TCanvas("dummy","",800,700);

  double B0 = 0.5;
  double B1 = 0.5;
  double B2 = 0.5;
  double Alpha_CB = 2.0;
  double N_CB = 2.0;
  double FSig = 0.9;

  for (unsigned int i=0; i<5; ++i) {

    TString sysName = sysNames[i];

    std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
    std::cout << std::endl;
    std::cout << "Systematics name : " << sysName << std::endl;
    std::cout << std::endl;

    TString nameHist = "mbb_tt";//+namesCAT[iCAT]+"_"+sysName;

    TH1D * hist = new TH1D(nameHist,"",NbinsSig,xmin,xmax);    
    treeTTbar[sysName]->Draw("mbb>>"+nameHist,"weight*("+cuts[iCAT]+")");
    mapNorm[sysName] = hist->GetSumOfWeights();
    delete hist;

    hist = new TH1D(nameHist,"",NbinsSig,xmin,xmax);
    treeTTbar[sysName]->Draw("mbb>>"+nameHist,"weight");

    RooRealVar mbbx("mbb","mass(bb)",xmin,xmax);
    RooRealVar meanx("mean","Mean",150,80,200);
    RooRealVar sigmax("sigma","Width",10,0,50);
    RooRealVar alphax("alpha","Alpha",2,0,50);
    RooRealVar nx("n","n",2,0,50);
    
    RooRealVar b0x("b0","b0",0.5,0,1);
    RooRealVar b1x("b1","b1",0.5,0,1);
    RooRealVar b2x("b2","b2",0.5,0,1);

    RooRealVar fsigx("fsig","fsig",0.5,0,1);

    if (!sysName.Contains("Nom")) {

      b0x.setVal(B0);
      b1x.setVal(B1);
      b2x.setVal(B2);
      alphax.setVal(Alpha_CB);
      nx.setVal(N_CB);
      fsigx.setVal(FSig);

      b0x.setConstant(kTRUE);
      b1x.setConstant(kTRUE);
      b2x.setConstant(kTRUE);
      alphax.setConstant(kTRUE);
      nx.setConstant(kTRUE);
      fsigx.setConstant(kTRUE);

    }

    RooBernstein BRNx("brn","Bernstein",mbbx,RooArgList(b0x,b1x,b2x));
    RooCBShape cbx("cb","CBshape",mbbx,meanx,sigmax,alphax,nx);
    RooGaussian gausx("gaus","Gauss",mbbx,meanx,sigmax);
    RooAddPdf signalx("signal","signal",RooArgList(gausx,BRNx),fsigx);

    RooDataHist data("data","data",mbbx,hist);
    RooFitResult * res = signalx.fitTo(data,Save(),SumW2Error(kTRUE));

    if (sysName.Contains("Nom")) {
      B0 = b0x.getValV();
      B1 = b1x.getValV();
      B2 = b2x.getValV();
      Alpha_CB = alphax.getValV();
      N_CB = nx.getValV();
      FSig = fsigx.getValV();
    }

    mapMean[sysName]  = meanx.getValV();
    mapSigma[sysName] = sigmax.getValV();

    delete hist;
    
  }
  delete dummy;

  RooRealVar mean("mean_tt_"+names[iCAT],"mean",140,80,200);
  RooRealVar sigma("sigma_tt_"+names[iCAT],"sigma",10,0,50);
  RooRealVar alpha("alpha_tt_"+names[iCAT],"alpha",2,0.,20.);
  RooRealVar n("n_tt_"+names[iCAT],"n",2,0,20);
  RooRealVar fsig("f_tt_"+names[iCAT],"fsig",0.5,0,1);
  RooRealVar b0("b0_tt_"+names[iCAT],"b0",0.5,0,1);
  RooRealVar b1("b1_tt_"+names[iCAT],"b1",0.5,0,1);
  RooRealVar b2("b2_tt_"+names[iCAT],"b2",0.5,0,1);
  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  double Mean = 0.5*(mapMean["JESUp"]+mapMean["JESDown"]);
  double d_Mean = mapMean["JESUp"] - Mean;
  
  double Sigma = 0.5*(mapSigma["JERUp"]+mapSigma["JERDown"]);
  double d_Sigma = mapSigma["JERUp"] - Sigma;

  mean.setVal(Mean);
  sigma.setVal(Sigma);
  alpha.setVal(Alpha_CB);
  n.setVal(N_CB);
  fsig.setVal(FSig);
  b0.setVal(B0);
  b1.setVal(B1);
  b2.setVal(B2);

  RooRealVar shift_JES("shift_JES_tt_"+names[iCAT],"shift_JES",d_Mean,d_Mean-1.0,d_Mean+1.0);
  RooRealVar shift_JER("shift_JER_tt_"+names[iCAT],"shift_JER",d_Sigma,d_Sigma-1.0,d_Sigma+1.0);

  RooFormulaVar mean_shifted("mean_shifted_tt_" +names[iCAT],
			     "@0 + @1*@2",
			     RooArgList(mean,theta_JES,shift_JES));

  RooFormulaVar sigma_shifted("sigma_shifted_tt_"+names[iCAT],
			      "@0 + @1*@2",
			      RooArgList(sigma,theta_JER,shift_JER));
    
  RooBernstein BRN_tt("brn_tt_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cb_tt("cb_tt_"+names[iCAT],"CBshape",mbb,mean_shifted,sigma_shifted,alpha,n);
  RooGaussian gaus_tt("gaus_tt_"+names[iCAT],"Gaussian",mbb,mean_shifted,sigma_shifted);
  RooAddPdf signal_tt("tt_"+names[iCAT],"TTbar",RooArgList(gaus_tt,BRN_tt),fsig);

  double norm_Central = mapNorm["Nom"];
  double d_norm_JES_Up = mapNorm["JESUp"] - mapNorm["Nom"];
  double d_norm_JES_Down = mapNorm["Nom"] - mapNorm["JESDown"];
  double d_norm_JER_Up = mapNorm["JERUp"] - mapNorm["Nom"];
  double d_norm_JER_Down = mapNorm["Nom"] - mapNorm["JERDown"];

  RooRealVar norm_Nom("norm_Nom_tt_"+names[iCAT],"norm_Nom",norm_Central,0.,2.*norm_Central);
  RooRealVar norm_JES_Up("norm_JES_Up_tt_"+names[iCAT],"norm_JES_Up",d_norm_JES_Up,-100.,100.);
  RooRealVar norm_JES_Down("norm_JES_Down_tt_"+names[iCAT],"norm_JES_Down",d_norm_JES_Down,-100.,100.);
  RooRealVar norm_JER_Up("norm_JER_Up_tt_"+names[iCAT],"norm_JER_Up",d_norm_JER_Up,-100.,100.);
  RooRealVar norm_JER_Down("norm_JER_Down_tt_"+names[iCAT],"norm_JER_Down",d_norm_JER_Down,-100.,100.);

  RooFormulaVar yield_tt("tt_"+names[iCAT]+"_norm",
			 "@0 + (@1>=0.0)*@1*@2 + (@1<0.0)*@1*@3 + (@4>=0.0)*@4*@5 + (@4<0.0)*@4*@6",
			 RooArgList(norm_Nom,theta_JES,norm_JES_Up,norm_JES_Down,theta_JER,norm_JER_Up,norm_JER_Down));

  alpha.setConstant(true);
  mean.setConstant(true);
  sigma.setConstant(true);
  n.setConstant(true);

  fsig.setConstant(true);

  b0.setConstant(true);
  b1.setConstant(true);
  b2.setConstant(true);

  shift_JES.setConstant(true);
  shift_JER.setConstant(true);

  norm_Nom.setConstant(true);
  norm_JES_Up.setConstant(true);
  norm_JES_Down.setConstant(true);
  norm_JER_Up.setConstant(true);
  norm_JER_Down.setConstant(true);
  
  outtext << "+++++++++++++ " << names[iCAT] << " ++++++++++++++" << endl;
  outtext << endl;
  outtext << "Mean  (ZJets) = " << mean_shifted.getVal() << endl;
  outtext << "Mean  (nom)   = " << mapMean["Nom"] << endl;
  outtext << "Mean  (ave)   = " << Mean << endl;
  outtext << "Mean  (Up)    = " << mapMean["JESUp"] << endl;
  outtext << "Mean  (Down)  = " << mapMean["JESDown"] << endl;
  outtext << endl;
  outtext << "Sigma (ZJets) = " << sigma_shifted.getVal() << endl;
  outtext << "Sigma (nom)   = " << mapSigma["Nom"] << endl;
  outtext << "Sigma (ave)   = " << Sigma << endl;
  outtext << "Sigma (Up)    = " << mapSigma["JERUp"] << endl;
  outtext << "Sigma (Down)  = " << mapSigma["JERDown"] << endl;
  outtext << endl;
  outtext << "Norm      = " << mapNorm["Nom"] << endl;
  outtext << "JESUp     = " << mapNorm["JESUp"] << endl;
  outtext << "JESDown   = " << mapNorm["JESDown"] << endl;
  outtext << "JERUp     = " << mapNorm["JERUp"] << endl;
  outtext << "JERDown   = " << mapNorm["JERDown"] << endl;
  outtext << endl;

  w->import(signal_tt);
  w->import(yield_tt);

}

void CreateTTbarTemplates_sys() {
  
  std::map<TString,TNtuple *> treeTTbar;

  for (int i=0; i<5; ++i) {
    TFile * file = new TFile(dirName+"/"+FileNamesBDT[i]);
    TString sysName = sysNames[i];
    treeTTbar[sysName] = (TNtuple*)file->Get("Mass_and_BDT_tt");
  }

  TFile * fileOutput = new TFile("rootshape/tt_singleb_shapes.root","recreate");
  //TFile * fileOutput = new TFile("workspace_singleb_powheg/tt_singleb_shapes.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","signal");

  RooRealVar theta_JES("CMS_JES_2018","CMS_JES_2018",0.,-5.,5.);
  RooRealVar theta_JER("CMS_JER_2018","CMS_JER_2018",0.,-5.,5.);

  for (int i=0; i<5; ++i) 
    CreateSignalPDF(i,treeTTbar,theta_JES,theta_JER,w);

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
