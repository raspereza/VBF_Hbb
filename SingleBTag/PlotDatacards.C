void PlotDatacards(TString fileName = "data_singleb_shapes.root",
		   TString fileNameSig = "signal_singleb_shapes.root",
		   TString catName = "CAT5") {

  using namespace RooFit;

  TFile * file = new TFile(fileName);
  RooWorkspace * w = (RooWorkspace*)file->Get("w");
  RooAbsData * data = w->data("data_"+catName);
  RooAbsPdf * pdf = w->pdf("qcd_"+catName);
  RooRealVar * mbb = w->var("mbb_"+catName);
  RooPlot* frame = mbb->frame();
  int nBins = data->numEntries();

  RooRealVar * yield = w->var("qcd_yield_"+catName);

  cout << "Yield = " << yield->getValV() << "+/-" << TMath::Sqrt(yield->getValV()) << endl;

  data->plotOn(frame);
  pdf->plotOn(frame);
  frame->Draw();

}
