void PlotDatacards2016() {

  TFile * file = new TFile("example/data_shapes_workspace.root");
  RooWorkspace * w = (RooWorkspace*)file->Get("w");
  RooAbsData * data = w->data("data_hist_CAT8");
  RooAbsPdf * pdf = w->pdf("qcd_model_POL4POL3_CAT8");
  RooRealVar * mbb = w->var("mbbReg_CAT8");
  RooPlot* frame = mbb->frame();
  int nBins = data->numEntries();
  std::cout << "Number of bins = " << nBins << std::endl; 
  data->plotOn(frame);
  pdf->plotOn(frame);
  frame->Draw();

}
