{
  TChain *ch=new TChain("gen_tree");
  ch->Add("*.root");
  //ch->Add("gen_run2[7-9][0-9].root");
  //ch->Add("gen_run32[0-6].root");
  //ch->Add("gen_run3[0-1][0-9].root");
  ch->Process("clean.C++g");
  gROOT->ProcessLine(".L util_new.cc");
  gROOT->ProcessLine(".L dsp.cc");
  gROOT->ProcessLine(".L binshift.cc");



}
