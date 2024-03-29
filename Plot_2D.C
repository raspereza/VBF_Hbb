#include "HttStylesNew.cc"

std::map<TString, TGraph*> Find_2D(int nPoints, // sqrt(number_of_points) 
				   double x1, // lower boundary of r_qqH
				   double x2,  // upper boundary of r_qqH
				   double y1, // lower boundary of r_ggH
				   double y2, // upper boundar of r_ggH
				   TTree * limit, // tree
				   double dnll, // 1sigma = 1, 2ssigma = 4 
				   double dLower = 0.5, // adjustments
				   double dUpper = 0.5  // adjustments
				   ) { 


  double deltaY = (y2-y1)/double(nPoints);

  TCanvas * dummy = new TCanvas("dummy","",600,500);
  TH2D * hist   = new TH2D("hist",  "",nPoints,x1,x2,nPoints,y1,y2);
  TH2D * update = new TH2D("update","",nPoints,x1,x2,nPoints,y1,y2);
  limit->Draw("r_ggH:r_qqH>>hist","2*deltaNLL");
  delete dummy;

  double xmin[100];
  double ymin[100];
  double xmax[100];
  double ymax[100];

  for (int i=1; i<=nPoints; ++i) {
    for (int j=1; j<=nPoints; ++j) {
      double value = hist->GetBinContent(i,j);
      double x = hist->GetXaxis()->GetBinCenter(i);
      double y = hist->GetYaxis()->GetBinCenter(j);
      if (value<1e-7) {
	value = 0.5*(hist->GetBinContent(i-1,j)+
		     hist->GetBinContent(i+1,j));
      }
      //      printf("[%5.2f,%5.2f] : %5.2f\n",x,y,value);
      update->SetBinContent(i,j,value);
    }
  }


  unsigned int imin = 0;
  unsigned int imax = 0;

  for (int i=1; i<=nPoints; ++i) {
    double y = update->GetYaxis()->GetBinCenter(i);
    for (int j=2; j<=nPoints-1; ++j) {
      double x1 = update->GetXaxis()->GetBinCenter(j-1);
      double x2 = update->GetXaxis()->GetBinCenter(j);
      double n1 = update->GetBinContent(j-1,i);
      double n2 = update->GetBinContent(j,i);
      if (n1<dnll&&n2>dnll) {
	double x = x1 + (dnll-n1)*(x2-x1)/(n2-n1);
	xmax[imax] = x;
	ymax[imax] = y;
	imax += 1;
      }
      if (n1>dnll&&n2<dnll) {
	double x = x1 + (dnll-n1)*(x2-x1)/(n2-n1);
	xmin[imin] = x;
	ymin[imin] = y;
	imin += 1;
      }
    }
  }	  
  std::cout << std::endl;
  for (unsigned int i=0; i<imin; ++i) 
    printf("%6.3f : %6.3f - %6.3f \n",ymin[i],xmin[i],xmax[i]);
  std::cout << std::endl;

  TGraph * GraphMin = new TGraph(imin,xmin,ymin);
  TGraph * GraphMax = new TGraph(imax,xmax,ymax);


  double xupper[4], yupper[4];
  double xlower[4], ylower[4];

  double xLowerMean = 0.5*(xmax[0] + xmin[0]);
  double xLowerDiff = xmax[0] - xmin[0];
  double yLower = ymin[0] - dUpper*deltaY;

  xlower[0] = xmin[0]; ylower[0] = ymin[0];
  xlower[1] = xLowerMean - 0.2*xLowerDiff; ylower[1] = yLower;
  xlower[2] = xLowerMean + 0.2*xLowerDiff; ylower[2] = yLower;
  xlower[3] = xmax[0]; ylower[3] = ymin[0];

  double xUpperMean = 0.5*(xmax[imax-1] + xmin[imin-1]);
  double xUpperDiff = xmax[imax-1] - xmin[imin-1];
  double yUpper = ymax[imax-1] + dLower*deltaY;

  xupper[0] = xmin[imin-1]; yupper[0] = ymin[imin-1];
  xupper[1] = xUpperMean - 0.2*xUpperDiff; yupper[1] = yUpper;
  xupper[2] = xUpperMean + 0.2*xUpperDiff; yupper[2] = yUpper;
  xupper[3] = xmax[imax-1]; yupper[3] = ymin[imin-1];
  
  TGraph * GraphLower = new TGraph(4,xlower,ylower);
  TGraph * GraphUpper = new TGraph(4,xupper,yupper);


  delete hist;
  delete update;

  std::map<TString, TGraph*> map_graph;
  map_graph["Max"] = GraphMax;
  map_graph["Min"] = GraphMin;
  map_graph["Lower"] = GraphLower;
  map_graph["Upper"] = GraphUpper;

  return map_graph;

}

// ++++++++++++++++++++++
// +++ Main subroutine
// ++++++++++++++++++++++

void Plot_2D( ) {

  SetStyle();

  TFile * fileObs = new TFile("./Fits2D_paper/2Dscan_obs.root");
  TTree * treeObs = (TTree*)fileObs->Get("limit");

  TFile * fileExp = new TFile("./Fits2D_paper/2Dscan_exp.root");
  TTree * treeExp = (TTree*)fileExp->Get("limit");

  double xmin_frame = -2;
  double xmax_frame =  4;

  double ymin_frame = -15;
  double ymax_frame =  15;

  double xbest[1] = {1.6};
  double ybest[1] = {-2.7};

  double xSM[1] = {1.0};
  double ySM[1] = {1.0};

  // Best fit 
  TGraph * graphBest = new TGraph(1,xbest,ybest);
  graphBest->SetMarkerStyle(43);
  graphBest->SetMarkerSize(4.0);
  graphBest->SetMarkerColor(kRed);

  // SM
  TGraph * graphSM = new TGraph(1,xSM,ySM);
  graphSM->SetMarkerStyle(43);
  graphSM->SetMarkerSize(4.0);
  graphSM->SetMarkerColor(kBlue);

  // ******************
  // observed contours
  // ******************
  double xmin = -1.0;
  double xmax = 5.0;
  double ymin = -15.;
  double ymax = 15.;
  int nPoints = 100;
  double dnll = 1.0;
  double dLower = 0.5;
  double dUpper = 0.5;

  std::map<TString, TGraph*> map_obs68 = 
    Find_2D(nPoints,xmin,xmax,ymin,ymax,treeObs,dnll,dLower,dUpper);
  for (auto graphs : map_obs68) {
    graphs.second->SetLineColor(2);
    graphs.second->SetLineWidth(3);
  }
  TGraph * Graph68_obs = map_obs68["Max"];

  dLower = 0.2;
  dUpper = 0.2;
  dnll = 4.0;
  std::map<TString, TGraph*> map_obs95 = 
    Find_2D(nPoints,xmin,xmax,ymin,ymax,treeObs,dnll,dLower,dUpper);
  for (auto graphs : map_obs95) {
    graphs.second->SetLineColor(2);
    graphs.second->SetLineWidth(3);
    graphs.second->SetLineStyle(2);
  }
  TGraph * Graph95_obs = map_obs95["Max"];

  // *****************
  // expected contours
  // *****************
  xmin = -1.0;
  xmax = 3.0;
  ymin = -9.;
  ymax = 11.;
  nPoints = 100;
  dnll = 1.0;
  dLower = 0.5;
  dUpper = 0.5;

  std::map<TString, TGraph*> map_exp68 = 
    Find_2D(nPoints,xmin,xmax,ymin,ymax,treeExp,dnll,dLower,dUpper);
  for (auto graphs : map_exp68) {
    graphs.second->SetLineColor(4);
    graphs.second->SetLineWidth(3);
  }
  TGraph * Graph68_exp = map_exp68["Max"];

  dLower = 0.2;
  dUpper = 0.2;
  dnll = 4.0;
  std::map<TString, TGraph*> map_exp95 = 
    Find_2D(nPoints,xmin,xmax,ymin,ymax,treeExp,dnll,dLower,dUpper);
  for (auto graphs : map_exp95) {
    graphs.second->SetLineColor(4);
    graphs.second->SetLineWidth(3);
    graphs.second->SetLineStyle(2);
  }
  TGraph * Graph95_exp = map_exp95["Max"];



  TH2D * frame = new TH2D("frame","",2,xmin_frame,xmax_frame,2,ymin_frame,ymax_frame);
  frame->GetXaxis()->SetTitle("r_{qqH}");
  frame->GetYaxis()->SetTitle("r_{ggH}");
  frame->GetXaxis()->SetTitleSize(0.06);
  frame->GetYaxis()->SetTitleSize(0.06);
  frame->GetXaxis()->SetNdivisions(206);

  TCanvas * canv = MakeCanvas("canv","",700,700);    
  frame->Draw();

  for (auto graphs : map_obs68) {
    graphs.second->Draw("lsame");
  }
  for (auto graphs : map_obs95) {
    graphs.second->Draw("lsame");
  }
  for (auto graphs : map_exp68) {
    graphs.second->Draw("lsame");
  }
  for (auto graphs : map_exp95) {
    graphs.second->Draw("lsame");
  }

  graphSM->Draw("psame");
  graphBest->Draw("psame");
  canv->SetGridx(true);
  canv->SetGridy(true);
  TLine * line1 = new TLine(xmin_frame,1.,xmax_frame,1.);
  line1->SetLineColor(kBlue);
  line1->SetLineWidth(2);
  line1->SetLineStyle(2);
  line1->Draw();
  TLine * line2 = new TLine(1.,ymin_frame,1.,ymax_frame);
  line2->SetLineColor(kBlue);
  line2->SetLineWidth(2);
  line2->SetLineStyle(2);
  line2->Draw();
  TLegend * leg = new TLegend(0.21,0.16,0.45,0.5);
  //  SetLegendStyle(leg);
  leg->SetTextSize(0.03);
  leg->AddEntry(graphBest,"best fit","p");
  leg->AddEntry(graphSM,"SM","p");
  leg->AddEntry(Graph68_obs,"obs 68% C.L.","l");
  leg->AddEntry(Graph95_obs,"obs 95% C.L.","l");
  leg->AddEntry(Graph68_exp,"exp 68% C.L.","l");
  leg->AddEntry(Graph95_exp,"exp 95% C.L.","l");
  leg->Draw();
  canv->Update();
  canv->Print("2D_scan.png");
  canv->Print("2D_scan.pdf");

}
