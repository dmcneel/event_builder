void allcores(Int_t RUNNUM){

  gROOT->ProcessLine(".L builder.C++");
  TString name("../root_data/run");
  name+=RUNNUM;
  name+=".root";
  // gROOT->ProcessLine(".L sort3.C++g");
  TChain *chain=new TChain("tree");
  chain->Add(name);
   TProof::Open("workers=14");
    chain->SetProof();
    chain->Process("builder.C+","of=gen.root;stf",50000000);
}
