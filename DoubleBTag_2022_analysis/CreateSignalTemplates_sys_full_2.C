#include "Common.h"

using namespace RooFit;

std::ofstream outtext("signal_out_full.txt");

void CreateSignalPDF(int iCAT,
		     std::map<TString, TNtuple*> treeGGH,
		     std::map<TString, TNtuple*> treeVBF,
		     RooRealVar theta_JES,
		     RooRealVar theta_JER,
		     RooRealVar theta_SCL,
		     RooRealVar theta_SMR,
		     RooWorkspace * w) {

  TH1::SetDefaultSumw2(true);

  std::map<TString,double> mapMean;
  std::map<TString,double> mapSigma;
  std::map<TString,double> mapNormGGH;
  std::map<TString,double> mapNormVBF;

  TCanvas * dummy = new TCanvas("dummy","",800,700);

  double B0 = 0.5;
  double B1 = 0.5;
  double B2 = 0.5;
  double Alpha_CB = 2.0;
  double N_CB = 2.0;
  double FSig = 0.9;

  for (unsigned int i=0; i<9; ++i) {

    TString sysName = sysNames[i];

    std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
    std::cout << std::endl;
    std::cout << "Systematics name : " << sysName << std::endl;
    std::cout << std::endl;

    TString nameGGHHist = "mbb_ggh";//+namesCAT[iCAT]+"_"+sysName;
    TString nameVBFHist = "mbb_vbf";//+namesCAT[iCAT]+"_"+sysName;

    TH1D * histGGH = new TH1D(nameGGHHist,"",NbinsSig,xmin,xmax);
    TH1D * histVBF = new TH1D(nameVBFHist,"",NbinsSig,xmin,xmax);
    
    treeGGH[sysName]->Draw(variable+">>"+nameGGHHist,"T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_central*T_online_btag_weight*("+cuts[iCAT]+")");
    treeVBF[sysName]->Draw(variable+">>"+nameVBFHist,"T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_central*T_online_btag_weight*("+cuts[iCAT]+")");
    histGGH->Scale(luminosity);
    histVBF->Scale(luminosity);
    std::cout << "These are very important" << std::endl;
    std::cout << "qqH Total: " << histVBF->GetSumOfWeights() <<  endl;
    std::cout << "ggH Total: " << histGGH->GetSumOfWeights() <<  endl;    
    outtext << "qqH Total: " << histVBF->GetSumOfWeights() <<  endl;
    outtext << "ggH Total: " << histGGH->GetSumOfWeights() <<  endl;

    mapNormGGH[sysName] = histGGH->GetSumOfWeights();
    mapNormVBF[sysName] = histVBF->GetSumOfWeights();
    
    delete histGGH;
    delete histVBF;

    histGGH = new TH1D(nameGGHHist,"",NbinsSig,xmin,xmax);
    histVBF = new TH1D(nameVBFHist,"",NbinsSig,xmin,xmax);
    
    treeGGH[sysName]->Draw(variable+">>"+nameGGHHist,"T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_central*T_online_btag_weight");
    treeVBF[sysName]->Draw(variable+">>"+nameVBFHist,"T_weight*LUMI*T_HLTweight*T_PUweight*T_btag_weight_central*T_online_btag_weight");
    histGGH->Scale(luminosity);
    histVBF->Scale(luminosity);

    RooRealVar mbbx("mbb","mass(bb)",xmin,xmax);

    RooRealVar meanx("mean","Mean",125,80,200);
    RooRealVar sigmax("sigma","Width",10,0,30);
    RooRealVar alphax("alpha","Alpha",0.9205);
    RooRealVar nx("n","n",2.78,0,10);
    
    RooRealVar b0x("b0","b0",0,1);
    RooRealVar b1x("b1","b1",0,1);
    RooRealVar b2x("b2","b2",0,1);

    RooRealVar fsigx("fsig","fsig",0,1);

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
    RooAddPdf signalx("signal","signal",RooArgList(cbx,BRNx),fsigx);

    histVBF->Add(histGGH,histVBF);
    RooDataHist data("dataGGH","dataGGH",mbbx,histVBF);
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

    delete histGGH;
    delete histVBF;
    
  }
  delete dummy;

  RooRealVar mean("mean_sig_"+names[iCAT],"mean",125,80,200);
  RooRealVar sigma("sigma_sig_"+names[iCAT],"sigma",10,0,30);
  RooRealVar alpha("alpha_sig_"+names[iCAT],"alpha",0.9205);
  RooRealVar n("n_sig_"+names[iCAT],"n",2.78,0,10);
  RooRealVar fsig("f_sig_"+names[iCAT],"fsig",0,1);
  RooRealVar b0("b0_sig_"+names[iCAT],"b0",0,1);
  RooRealVar b1("b1_sig_"+names[iCAT],"b1",0,1);
  RooRealVar b2("b2_sig_"+names[iCAT],"b2",0,1);
  RooRealVar mbb("mbb_"+names[iCAT],"mass(bb)",xmin,xmax);

  double Mean = 0.5*(mapMean["JESUp"]+mapMean["JESDown"]);
  double d_Mean = mapMean["JESUp"] - Mean;
  
  double Sigma = 0.5*(mapSigma["JERUp"]+mapSigma["JERDown"]);
  double d_Sigma = mapSigma["JERUp"] - Sigma;

  double Mean2 = 0.5*(mapMean["PNetJECUp"]+mapMean["PNetJECDown"]);
  double d_Mean2 = mapMean["PNetJECUp"] - Mean2;

  double Sigma2 = 0.5*(mapSigma["SmearUp"]+mapSigma["SmearDown"]);
  double d_Sigma2 = mapSigma["SmearUp"] - Sigma2;
  
  mean.setVal(Mean);
  sigma.setVal(Sigma);
  alpha.setVal(Alpha_CB);
  n.setVal(N_CB);
  fsig.setVal(FSig);
  b0.setVal(B0);
  b1.setVal(B1);
  b2.setVal(B2);

  RooRealVar shift_JES("shift_JES_sig_"+names[iCAT],"shift_JES",d_Mean,d_Mean-5.0,d_Mean+5.0);
  RooRealVar shift_JER("shift_JER_sig_"+names[iCAT],"shift_JER",d_Sigma,d_Sigma-1.0,d_Sigma+1.0);
  RooRealVar shift_SCL("shift_SCL_sig_"+names[iCAT],"shift_SCL",d_Mean2,d_Mean2-5.0,d_Mean2+5.0);
  RooRealVar shift_SMR("shift_SMR_sig_"+names[iCAT],"shift_SMR",d_Sigma2,d_Sigma2-1.0,d_Sigma2+1.0);

  RooFormulaVar mean_shifted_ggH("mean_shifted_ggH_" +names[iCAT],
				 "@0 + @1*@2 + @3*@4",
				 RooArgList(mean,theta_JES,shift_JES,theta_SCL,shift_SCL));
  RooFormulaVar mean_shifted_qqH("mean_shifted_qqH_" +names[iCAT],
				 "@0 + @1*@2 + @3*@4",
				 RooArgList(mean,theta_JES,shift_JES,theta_SCL,shift_SCL));
  RooFormulaVar sigma_shifted_ggH("sigma_shifted_ggH_"+names[iCAT],
				  "@0 + @1*@2 + @3*@4",
				  RooArgList(sigma,theta_JER,shift_JER,theta_SMR,shift_SMR));
  RooFormulaVar sigma_shifted_qqH("sigma_shifted_qqH_"+names[iCAT],
				  "@0 + @1*@2 + @3*@4",
				  RooArgList(sigma,theta_JER,shift_JER,theta_SMR,shift_SMR));



  RooBernstein BRN_ggH("brn_ggH_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cb_ggH("cb_ggH_"+names[iCAT],"CBshape",mbb,mean_shifted_ggH,sigma_shifted_ggH,alpha,n);
  RooGaussian gaus_ggH("gaus_ggH_"+names[iCAT],"Gaussian",mbb,mean_shifted_ggH,sigma_shifted_ggH);
  RooAddPdf signal_ggH("ggH_"+names[iCAT],"ggH",RooArgList(cb_ggH,BRN_ggH),fsig);


  RooBernstein BRN_qqH("brn_qqH_"+names[iCAT],"Bernstein",mbb,RooArgList(b0,b1,b2));
  RooCBShape cb_qqH("cb_qqH_"+names[iCAT],"CBshape",mbb,mean_shifted_qqH,sigma_shifted_qqH,alpha,n);
  RooGaussian gaus_qqH("gaus_qqH_"+names[iCAT],"Gaussian",mbb,mean_shifted_qqH,sigma_shifted_qqH);
  RooAddPdf signal_qqH("qqH_"+names[iCAT],"qqH",RooArgList(cb_qqH,BRN_qqH),fsig);

  double normGGH_Central = mapNormGGH["Nom"];
  double normGGH_mean_JES = 0.5*(mapNormGGH["JESUp"]+mapNormGGH["JESDown"]); 
  double normGGH_mean_JER = 0.5*(mapNormGGH["JERUp"]+mapNormGGH["JERDown"]);
  double normGGH_mean_SCL = 0.5*(mapNormGGH["PNetJECUp"]+mapNormGGH["PNetJECDown"]);
  double normGGH_mean_SMR = 0.5*(mapNormGGH["SmearUp"]+mapNormGGH["SmearDown"]);
  double normGGH_shift_JES = (mapNormGGH["JESUp"]/normGGH_mean_JES -1.0)*normGGH_Central;
  double normGGH_shift_JER = (mapNormGGH["JERUp"]/normGGH_mean_JER -1.0)*normGGH_Central;
  double normGGH_shift_SCL = (mapNormGGH["PNetJECUp"]/normGGH_mean_SCL -1.0)*normGGH_Central;
  double normGGH_shift_SMR = (mapNormGGH["SmearUp"]/normGGH_mean_SMR -1.0)*normGGH_Central;
  

  double normVBF_Central = mapNormVBF["Nom"];
  double normVBF_mean_JES = 0.5*(mapNormVBF["JESUp"]+mapNormVBF["JESDown"]); 
  double normVBF_mean_JER = 0.5*(mapNormVBF["JERUp"]+mapNormVBF["JERDown"]);
  double normVBF_mean_SCL = 0.5*(mapNormVBF["PNetJECUp"]+mapNormVBF["PNetJECDown"]);
  double normVBF_mean_SMR = 0.5*(mapNormVBF["SmearUp"]+mapNormVBF["SmearDown"]);
  double normVBF_shift_JES = (mapNormVBF["JESUp"]/normVBF_mean_JES -1.0)*normVBF_Central;
  double normVBF_shift_JER = (mapNormVBF["JERUp"]/normVBF_mean_JER -1.0)*normVBF_Central;
  double normVBF_shift_SCL = (mapNormVBF["PNetJECUp"]/normVBF_mean_SCL -1.0)*normVBF_Central;
  double normVBF_shift_SMR = (mapNormVBF["SmearUp"]/normVBF_mean_SMR -1.0)*normVBF_Central;


  RooRealVar NormGGH_Nom("norm_Nom_ggH_"+names[iCAT],"norm_Nom_ggH",normGGH_Central,0.,2.*normGGH_Central);
  RooRealVar NormGGH_shift_JES("norm_shift_JES_ggH_"+names[iCAT],"norm_shift_JES_ggH",normGGH_shift_JES,normGGH_shift_JES-10.,normGGH_shift_JES+10.);
  RooRealVar NormGGH_shift_JER("norm_shift_JER_ggH_"+names[iCAT],"norm_shift_JER_ggH",normGGH_shift_JER,normGGH_shift_JER-10.,normGGH_shift_JER+10.);
  RooRealVar NormGGH_shift_SCL("norm_shift_SCL_ggH_"+names[iCAT],"norm_shift_SCL_ggH",normGGH_shift_SCL,normGGH_shift_SCL-10.,normGGH_shift_SCL+10.);
  RooRealVar NormGGH_shift_SMR("norm_shift_SMR_ggH_"+names[iCAT],"norm_shift_SMR_ggH",normGGH_shift_SMR,normGGH_shift_SMR-10.,normGGH_shift_SMR+10.);


  RooRealVar NormVBF_Nom("norm_Nom_qqH_"+names[iCAT],"norm_Nom_qqH",normVBF_Central,0.,2.*normVBF_Central);
  RooRealVar NormVBF_shift_JES("norm_shift_JES_qqH_"+names[iCAT],"norm_shift_JES_qqH",normVBF_shift_JES,normVBF_shift_JES-10.,normVBF_shift_JES+10.);
  RooRealVar NormVBF_shift_JER("norm_shift_JER_qqH_"+names[iCAT],"norm_shift_JER_qqH",normVBF_shift_JER,normVBF_shift_JER-10.,normVBF_shift_JER+10.);
  RooRealVar NormVBF_shift_SCL("norm_shift_SCL_qqH_"+names[iCAT],"norm_shift_SCL_qqH",normVBF_shift_SCL,normVBF_shift_SCL-10.,normVBF_shift_SCL+10.);
  RooRealVar NormVBF_shift_SMR("norm_shift_SMR_qqH_"+names[iCAT],"norm_shift_SMR_qqH",normVBF_shift_SMR,normVBF_shift_SMR-10.,normVBF_shift_SMR+10.);

  RooFormulaVar yield_GGH("ggH_"+names[iCAT]+"_norm",
			  "@0 + @1*@2 + @3*@4",
			  RooArgList(NormGGH_Nom,theta_JES,NormGGH_shift_JES,theta_JER,NormGGH_shift_JER));
  RooFormulaVar yield_VBF("qqH_"+names[iCAT]+"_norm",
			  "@0 + @1*@2 + @3*@4",
			  RooArgList(NormVBF_Nom,theta_JES,NormVBF_shift_JES,theta_JER,NormVBF_shift_JER));

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
  shift_SCL.setConstant(true);
  shift_SMR.setConstant(true);

  NormGGH_Nom.setConstant(true);
  NormGGH_shift_JES.setConstant(true);
  NormGGH_shift_JER.setConstant(true);
  NormGGH_shift_SCL.setConstant(true);
  NormGGH_shift_SMR.setConstant(true);

  NormVBF_Nom.setConstant(true);
  NormVBF_shift_JES.setConstant(true);
  NormVBF_shift_JER.setConstant(true);
  NormVBF_shift_SCL.setConstant(true);
  NormVBF_shift_SMR.setConstant(true);
  
  outtext << "+++++++++++++ " << names[iCAT] << " ++++++++++++++++" << endl;
  outtext << "+++++++++++++ start unc ++++++++++++++++" << endl;
  outtext << "Mean  = " << Mean << endl;
  outtext << "Mean2  = " << Mean2 << endl;
  outtext << "Mean  (nom)  = " << mapMean["Nom"] << endl;
  outtext << "Sigma (nom)  = " << mapSigma["Nom"] << endl;
  outtext << "Sigma  = " << Sigma << endl;
  outtext << "Sigma2  = " << Sigma2 << endl;
  outtext << "d_Mean  = " << d_Mean << endl;
  outtext << "d_Mean2  = " << d_Mean2 << endl;
  outtext << "d_Sigma  = " << d_Sigma << endl;
  outtext << "d_Sigma2  = " << d_Sigma2 << endl;

  outtext << "+++++++++++++ end unc ++++++++++++++++" << endl;

  outtext << endl;
  outtext << "Mean  (ggH)  = " << mean_shifted_ggH.getVal() << endl;
  outtext << "Mean  (qqH)  = " << mean_shifted_qqH.getVal() << endl;
  outtext << "Mean  (nom)  = " << mapMean["Nom"] << endl;
  outtext << "Mean  (ave)  = " << Mean << endl;
  outtext << "Mean  (Up)   = " << mapMean["JESUp"] << endl;
  outtext << "Mean  (Down) = " << mapMean["JESDown"] << endl;
  outtext << "SCL Mean  (Up)   = " << mapMean["PNetJECUp"] << endl;
  outtext << "SCL Mean  (Down) = " << mapMean["PNetJECDown"] << endl;
  outtext << "SMR Mean  (Up)   = " << mapMean["SmearUp"] << endl;
  outtext << "SMR Mean  (Down) = " << mapMean["SmearDown"] << endl;    
  //*****************************************************//
  outtext << endl;
  outtext << "Sigma (ggH)  = " << sigma_shifted_ggH.getVal() << endl;
  outtext << "Sigma (qqH)  = " << sigma_shifted_qqH.getVal() << endl;
  outtext << "Sigma (nom)  = " << mapSigma["Nom"] << endl;
  outtext << "Sigma (ave)  = " << Sigma << endl;
  outtext << "Sigma (Up)   = " << mapSigma["JERUp"] << endl;
  outtext << "Sigma (Down) = " << mapSigma["JERDown"] << endl;
  outtext << "SCL Sigma (Up)   = " << mapSigma["PNetJECUp"] << endl;
  outtext << "SCL Sigma (Down) = " << mapSigma["PNetJECDown"] << endl;
  outtext << "SMR Sigma (Up)   = " << mapSigma["SmearUp"] << endl;
  outtext << "SMR Sigma (Down) = " << mapSigma["SmearDown"] << endl;
  //******************************************************//

  outtext << endl;
  outtext << "Norm    (ggH)  = " << mapNormGGH["Nom"] << endl;
  outtext << "JESUp   (ggH)  = " << mapNormGGH["JESUp"] << endl;
  outtext << "JESDown (ggH)  = " << mapNormGGH["JESDown"] << endl;
  outtext << "JERUp   (ggH)  = " << mapNormGGH["JERUp"] << endl;
  outtext << "JERDown (ggH)  = " << mapNormGGH["JERDown"] << endl;
  outtext << "SCLUp   (ggH)  = " << mapNormGGH["PNetJECUp"] << endl;
  outtext << "SCLDown (ggH)  = " << mapNormGGH["PNetJECDown"] << endl;
  outtext << "SMRUp   (ggH)  = " << mapNormGGH["SmearUp"] << endl;
  outtext << "SMRDown (ggH)  = " << mapNormGGH["SmearDown"] << endl;
  //******************************************************//
  outtext << endl;
  outtext << "Norm    (qqH)  = " << mapNormVBF["Nom"] << endl;
  outtext << "JESUp   (qqH)  = " << mapNormVBF["JESUp"] << endl;
  outtext << "JESDown (qqH)  = " << mapNormVBF["JESDown"] << endl;
  outtext << "JERUp   (qqH)  = " << mapNormVBF["JERUp"] << endl;
  outtext << "JERDown (qqH)  = " << mapNormVBF["JERDown"] << endl;
  outtext << "SCLUp   (qqH)  = " << mapNormVBF["PNetJECUp"] << endl;
  outtext << "SCLDown (qqH)  = " << mapNormVBF["PNetJECDown"] << endl;
  outtext << "SMRUp   (qqH)  = " << mapNormVBF["SmearUp"] << endl;
  outtext << "SMRDown (qqH)  = " << mapNormVBF["SmearDown"] << endl;
  
  outtext << endl;

  w->import(signal_ggH);
  w->import(signal_qqH);
  w->import(yield_GGH);
  w->import(yield_VBF);

}

void CreateSignalTemplates_sys_full_2() {
  
  std::map<TString,TNtuple *> treeGGH;
  std::map<TString,TNtuple *> treeVBF;

  for (int i=0; i<9; ++i) {
    TFile * file_vbfh = new TFile(dirName+"/"+FileNamesVBFDNN[i]);
    TFile * file_ggh = new TFile(dirName+"/"+FileNamesGGHDNN[i]);
    TString sysName = sysNames[i];
    treeVBF[sysName] = (TNtuple*)file_vbfh->Get("tree");
    treeGGH[sysName] = (TNtuple*)file_ggh->Get("tree");
  }
  TFile * fileOutput = new TFile("workspace/signal_resolved_vbfhbb_2022.root","recreate");
  fileOutput->cd("");
  RooWorkspace * w = new RooWorkspace("w","signal");

  RooRealVar theta_JES("CMS_JES_2022","CMS_JES_2022",0.,-5.,5.);
  RooRealVar theta_JER("CMS_JER_2022","CMS_JER_2022",0.,-5.,5.);
  RooRealVar theta_SCL("CMS_scale_b_2022","CMS_scale_b_2022",0.,-5.,5.);
  RooRealVar theta_SMR("CMS_res_b_2022","CMS_res_b_2022",0.,-5.,5.);

  for (int i=0; i<7; ++i) 
    CreateSignalPDF(i,treeGGH,treeVBF,theta_JES,theta_JER,theta_SCL,theta_SMR,w);

  w->Write("w");
  fileOutput->Write();
  fileOutput->Close();
}
