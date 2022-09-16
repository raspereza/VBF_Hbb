#include "HttStylesNew.cc"

void Plot_2D(int nPoints = 50, // sqrt(number_of_points) 
	     double x1 = -0.5, // lower boundary of r_qqH
	     double x2 = 2.5,  // upper boundary of r_qqH
	     double y1 = -15, // lower boundary of r_ggH
	     double y2 = 15, // upper boundar of r_ggH
	     TString fileName = "higgsCombine.datacards_singleb_Run2_EWK.MultiDimFit.mH125"
	     ) {

  SetStyle();

  double xmin_frame = -0.5;
  double xmax_frame =  2.5;

  double ymin_frame = -15;
  double ymax_frame =  15;

  double deltaY = (y2-y1)/double(nPoints);

  TFile * file = new TFile(fileName+".root");
  TCanvas * dummy = new TCanvas("dummy","",500,500);
  TTree * limit = (TTree*)file->Get("limit");
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
      if (n1<1.0&&n2>1.0) {
	double x = x1 + (1.0-n1)*(x2-x1)/(n2-n1);
	xmax[imax] = x;
	ymax[imax] = y;
	imax += 1;
      }
      if (n1>1.0&&n2<1.0) {
	double x = x1 + (1.0-n1)*(x2-x1)/(n2-n1);
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

  GraphMax->SetLineColor(2);
  GraphMax->SetLineWidth(3);
  GraphMin->SetLineColor(2);
  GraphMin->SetLineWidth(3);

  double xupper[4], yupper[4];
  double xlower[4], ylower[4];

  double xLowerMean = 0.5*(xmax[0] + xmin[0]);
  double xLowerDiff = xmax[0] - xmin[0];
  double yLower = ymin[0] - 0.5*deltaY;

  xlower[0] = xmin[0]; ylower[0] = ymin[0];
  xlower[1] = xLowerMean - 0.2*xLowerDiff; ylower[1] = yLower;
  xlower[2] = xLowerMean + 0.2*xLowerDiff; ylower[2] = yLower;
  xlower[3] = xmax[0]; ylower[3] = ymin[0];

  double xUpperMean = 0.5*(xmax[imax-1] + xmin[imin-1]);
  double xUpperDiff = xmax[imax-1] - xmin[imin-1];
  double yUpper = ymax[imax-1] + 0.7*deltaY;

  xupper[0] = xmin[imin-1]; yupper[0] = ymin[imin-1];
  xupper[1] = xUpperMean - 0.2*xUpperDiff; yupper[1] = yUpper;
  xupper[2] = xUpperMean + 0.2*xUpperDiff; yupper[2] = yUpper;
  xupper[3] = xmax[imax-1]; yupper[3] = ymin[imin-1];
  
  TGraph * GraphLower = new TGraph(4,xlower,ylower);
  TGraph * GraphUpper = new TGraph(4,xupper,yupper);

  GraphLower->SetLineColor(2);
  GraphLower->SetLineWidth(3);
  GraphUpper->SetLineWidth(3);
  GraphUpper->SetLineColor(2);

  update->GetXaxis()->SetRangeUser(xmin_frame,xmax_frame);
  update->GetYaxis()->SetRangeUser(ymin_frame,ymax_frame);

  double xbest[1] = {1.};
  double ybest[1] = {1.};

  TGraph * graphBest = new TGraph(1,xbest,ybest);
  graphBest->SetMarkerStyle(43);
  graphBest->SetMarkerSize(4.0);
  graphBest->SetMarkerColor(kBlue);

  TH2D * frame = new TH2D("frame","",2,xmin_frame,xmax_frame,2,ymin_frame,ymax_frame);
  frame->GetXaxis()->SetTitle("r_{qqH}");
  frame->GetYaxis()->SetTitle("r_{ggH}");
  frame->GetXaxis()->SetTitleSize(0.06);
  frame->GetYaxis()->SetTitleSize(0.06);

  TCanvas * canv = MakeCanvas("canv","",600,600);    
  frame->Draw();
  //  update->Draw("colzsame");
  GraphMax->Draw("lsame");
  GraphMin->Draw("lsame");
  GraphLower->Draw("lsame");
  GraphUpper->Draw("lsame");
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
  TLegend * leg = new TLegend(0.57,0.72,0.82,0.9);
  leg->SetTextSize(0.042);
  leg->AddEntry(graphBest,"best fit","p");
  leg->AddEntry(GraphMin,"68% C.L.","l");
  leg->Draw();
  canv->Update();
  canv->Print("2D_scan.png");


}
