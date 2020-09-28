void CreateMbbBdtNtuple(TString dir,
			TString name,
			TString tupleName) {

  Float_t minvqq,detaqq,dphiqq,btgb1_D,btgb2_D,qtgq1,qtgq2,NJ_ingap,pt_All,pz_All,Njet_20,HTT_rest_20,E_rest_20,minvbb;
  TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );
  reader->AddVariable( "mqq",                &minvqq );
  reader->AddVariable( "detaqq",             &detaqq );
  reader->AddVariable( "dphiqq",             &dphiqq );
  reader->AddVariable( "btgb1_D",            &btgb1_D );
  reader->AddVariable( "btgb2_D",            &btgb2_D );
  reader->AddVariable( "qtgq1",              &qtgq1 );
  reader->AddVariable( "qtgq2",              &qtgq2 );
  reader->AddVariable( "pt_All",             &pt_All );
  reader->AddVariable( "pz_All",             &pz_All );
  reader->AddVariable( "Njet_20",            &Njet_20 );
  reader->AddVariable( "E_rest_20",          &E_rest_20 );
  reader->AddSpectator( "minvbb",            &minvbb );

  reader->BookMVA("TMVAClassification_BDTG", "dataset/weights/TMVAClassification_BDTG.weights.xml" );

  TFile * file = new TFile(name+"_stat.root","recreate");
  file->cd("");
  Float_t weight_tree;
  Float_t bdt_tree;
  Float_t mbb_tree;
  TTree * tree = new TTree("Stat","Stat. Inference");
  tree->Branch("weight",&weight_tree,"weight/F");
  tree->Branch("bdt",&bdt_tree,"bdt/F");
  tree->Branch("mbb",&mbb_tree,"mbb/F");

  TFile * file0 = new TFile(dir+"/NTuples_"+name+"_.root");
  TTree * ntps0 = (TTree*)file0->Get(tupleName);
  Float_t My_weight,My_minvqq,My_detaqq,My_dphiqq,My_btgb1_D,My_btgb2_D,My_qtgq1,My_qtgq2,My_NJ_ingap,My_pt_All,My_pz_All,My_Njet_20,My_HTT_rest_20,My_E_rest_20,My_minvbb;

   ntps0->SetBranchAddress( "weight",                &My_weight );
   ntps0->SetBranchAddress( "mqq",                   &My_minvqq );
   ntps0->SetBranchAddress( "detaqq",                &My_detaqq );
   ntps0->SetBranchAddress( "dphiqq",                &My_dphiqq );
   ntps0->SetBranchAddress( "btgb1_D",               &My_btgb1_D );
   ntps0->SetBranchAddress( "btgb2_D",               &My_btgb2_D );
   ntps0->SetBranchAddress( "qtgq1",                 &My_qtgq1 );
   ntps0->SetBranchAddress( "qtgq2",                 &My_qtgq2 );
   ntps0->SetBranchAddress( "NJ_ingap",              &My_NJ_ingap );
   ntps0->SetBranchAddress( "pt_All",                &My_pt_All );
   ntps0->SetBranchAddress( "pz_All",                &My_pz_All );
   ntps0->SetBranchAddress( "Njet_20",               &My_Njet_20 );
   ntps0->SetBranchAddress( "HTT_rest_20",           &My_HTT_rest_20 );
   ntps0->SetBranchAddress( "E_rest_20",             &My_E_rest_20 );
   ntps0->SetBranchAddress( "minvbb",                &My_minvbb );
   //   std::cout << "Tree " << ntps0 << std::endl;

   int nEntries = ntps0->GetEntries();
   std::cout << "Running on " << name << " with entries = " << nEntries << std::endl;

   for (int iEntry=0; iEntry<nEntries; ++iEntry) {
     ntps0->GetEntry(iEntry);
     minvqq   = My_minvqq;
     detaqq   = My_detaqq;
     dphiqq   = My_dphiqq;
     btgb1_D  = My_btgb1_D;
     btgb2_D  = My_btgb2_D;
     qtgq1    = My_qtgq1;
     qtgq2    = My_qtgq2;
     NJ_ingap = My_NJ_ingap;
     pt_All   = My_pt_All;
     pz_All   = My_pz_All;
     Njet_20  = My_Njet_20;
     HTT_rest_20  = My_HTT_rest_20;
     E_rest_20    = My_E_rest_20;
     minvbb       = My_minvbb;
     
     mbb_tree = My_minvbb;
     weight_tree = My_weight;
     //     std::cout << "Weight = " << weight_tree << std::endl;
     bdt_tree = reader->EvaluateMVA("TMVAClassification_BDTG");
     tree->Fill();
   }
   file->cd("");
   tree->Write("Stat");
   file->Close();
}

