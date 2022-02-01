#include "HttStylesNew.cc"

void Plot_2D() {

  SetStyle();

  TFile * file = new TFile("higgsCombine.datacards_singleb_Run2_UL_v2.MultiDimFit.mH125.root");
  TCanvas * dummy = new TCanvas("dummy","",500,500);
  TTree * limit = (TTree*)file->Get("limit");
  TH2D * hist   = new TH2D("hist",  "",40,-0.5,2.5,40,-15.,15.);
  TH2D * update = new TH2D("update","",40,-0.5,2.5,40,-15.,15.);
  limit->Draw("r_ggH:r_qqH>>hist","2*deltaNLL");

  double xmin[100];
  double ymin[100];
  double xmax[100];
  double ymax[100];

  double xmin2[100];
  double ymin2[100];
  double xmax2[100];
  double ymax2[100];
  
  unsigned int imin = 0;
  unsigned int imax = 0;
  unsigned int imin2 = 0;
  unsigned int imax2 = 0;

  for (int i=1; i<=40; ++i) {
    for (int j=1; j<=40; ++j) {
      double value = hist->GetBinContent(i,j);
      double x = hist->GetXaxis()->GetBinCenter(i);
      double y = hist->GetYaxis()->GetBinCenter(j);
      if (value<1e-7) {
	value = 0.5*(hist->GetBinContent(i-1,j)+
		     hist->GetBinContent(i+1,j));
      }
      //      std::cout << "[" << x << "," << y << "] : " << value << std::endl;
      update->SetBinContent(i,j,value);
    }
  }


  for (int i=1; i<=40; ++i) {
    double y = update->GetYaxis()->GetBinCenter(i);
    for (int j=2; j<=40; ++j) {
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

      if (n1<4.0&&n2>4.0) {
	double x = x1 + (4.0-n1)*(x2-x1)/(n2-n1);
	xmax2[imax2] = x;
	ymax2[imax2] = y;
	imax2 += 1;
      }
      if (n1>4.0&&n2<4.0) {
	double x = x1 + (4.0-n1)*(x2-x1)/(n2-n1);
	xmin2[imin2] = x;
	ymin2[imin2] = y;
	imin2 += 1;
      }
    }
  }	  
  std::cout << std::endl;
  for (unsigned int i=0; i<imin; ++i) 
    std::cout << ymin[i] << ":" << xmin[i] << std::endl;
  std::cout << std::endl;
  for (unsigned int i=0; i<imax; ++i) 
    std::cout << ymax[i] << ":" << xmax[i] << std::endl;
  std::cout << std::endl;

  TGraph * GraphMin = new TGraph(imin,xmin,ymin);
  TGraph * GraphMax = new TGraph(imax,xmax,ymax);

  GraphMax->SetLineColor(2);
  GraphMax->SetLineWidth(3);
  GraphMin->SetLineColor(2);
  GraphMin->SetLineWidth(3);

  TGraph * GraphMin2 = new TGraph(imin2,xmin2,ymin2);
  TGraph * GraphMax2 = new TGraph(imax2,xmax2,ymax2);

  double xupper[2], yupper[2];
  double xlower[2], ylower[2];

  yupper[0] = ymin[0];
  yupper[1] = ymax[0];
  xupper[0] = xmin[0];
  xupper[1] = xmax[0];

  ylower[0] = ymin[imin-1];
  ylower[1] = ymax[imax-1];
  xlower[0] = xmin[imin-1];
  xlower[1] = xmax[imax-1];
  
  TGraph * GraphLower = new TGraph(2,xlower,ylower);
  TGraph * GraphUpper = new TGraph(2,xupper,yupper);
  

  GraphLower->SetLineColor(2);
  GraphLower->SetLineWidth(3);
  GraphUpper->SetLineWidth(3);
  GraphUpper->SetLineColor(2);

  update->GetXaxis()->SetRangeUser(-0.5,2.5);
  update->GetYaxis()->SetRangeUser(-14,14);

  double xbest[1] = {1.};
  double ybest[1] = {1.};

  TGraph * graphBest = new TGraph(1,xbest,ybest);
  graphBest->SetMarkerStyle(43);
  graphBest->SetMarkerSize(4.0);
  graphBest->SetMarkerColor(kBlue);

  TH2D * frame = new TH2D("frame","",2,-0.5,2.5,2,-15,15);
  frame->GetXaxis()->SetTitle("r_{qqH}");
  frame->GetYaxis()->SetTitle("r_{ggH}");
  frame->GetXaxis()->SetTitleSize(0.06);
  frame->GetYaxis()->SetTitleSize(0.06);

  delete dummy;
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
  TLine * line1 = new TLine(-0.5,1.,2.5,1.);
  line1->SetLineColor(kBlue);
  line1->SetLineWidth(2);
  line1->SetLineStyle(2);
  line1->Draw();
  TLine * line2 = new TLine(1,-15.,1,15.);
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
  canv->Print("2D.png");


}
