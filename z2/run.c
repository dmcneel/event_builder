{
  TChain *ch=new TChain("gen_tree");
  ch->Add("*.root");
  ch->Process("clean.C++g");
  gROOT->ProcessLine(".L util_new.cc");
  gROOT->ProcessLine(".L dsp.cc");
  gROOT->ProcessLine(".L binshift.cc");



}
