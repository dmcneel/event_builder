#include <TCanvas.h>
#include <iostream>
#include <TStyle.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TVirtualPad.h>
#include <TROOT.h>




Int_t rangelow=0;
Int_t rangehigh=0;
TF1 *f1=new TF1("f1","pol2",-850,-500);
TF1 *f2=new TF1("f2","pol3",-850,-500);
Int_t x;
Bool_t accepted;
Int_t type;
Int_t detid;
TCanvas *c1;

void fit(Int_t detid)
{
  


    rangelow=-850+59*(detid%6);
    rangehigh=-790+59*(detid%6);
    f1->SetRange(rangelow,rangehigh);
    f2->SetRange(rangelow,rangehigh);
    TString name("hezc");
    name+=detid;
    TH2F *hist=(TH2F*)gROOT->FindObject(name);
    hist->SetAxisRange(rangelow,rangehigh,"X");
    hist->SetAxisRange(0,8,"Y");
    hist->SetMarkerStyle(kFullCircle);
    hist->Draw();

 
  
   
  
    c1->WaitPrimitive("CUTG","CutG");
    TH2F *xprof1=(TH2F*)hist->Clone();
    hist->ProfileX("xprof1",0,512,"[CUTG");
  
    xprof1->Fit("f1","R");
    xprof1->Fit("f2","R");
    xprof1->Draw();
    f1->Draw("same");
    f2->Draw("same");
    c1->Update();
    c1->WaitPrimitive();

  
}
void fitall(void){
  c1=new TCanvas("c1","c1");
  gStyle->SetOptStat(0);
  cout<<"Press y to accept and move on, n to try again, and b to go back"<<endl;
  fit(0);
  c1->AddExec("acceptyn","acceptfit()");
 
     
}
void acceptfit(void){
  Int_t event=gPad->GetEvent();
  Int_t  x=gPad->GetEventX();
  switch(x){
  case 'y':
    detid++;
    fit(detid);
    break;
  case 'n':
    fit(detid);
    break;
  case 'b':
    detid--;
    fit(detid);
    break;
  case 'f':
    detid++;
    break;
  };
}
void endaccept(void){
  c1->DeleteExec("acceptyn");
  return;
}
