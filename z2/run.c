{
  TChain *ch=new TChain("gen_tree");
  ch->Add("*.root");
  ch->Process("clean.C++g");




}
