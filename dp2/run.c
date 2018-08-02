{
  TChain *ch=new TChain("gen_tree");
  ch->Add("gen_run40[1-3].root");
  ch->Process("clean.C++g");
  gROOT->ProcessLine(".L ezfit.c");
  gROOT->ProcessLine(".L checkcal.c");
  gROOT->ProcessLine(".L checkside.c");
  gROOT->ProcessLine(".L util_new.cc");
  gROOT->ProcessLine(".L dsp.cc");



}
