#define builder_cxx
// The class definition in builder.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("builder.C")
// root> T->Process("builder.C","some options")
// root> T->Process("builder.C+")
//

#include "builder.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <TH3.h>
#include <TGraph.h>
#include <TMath.h>
#include <TLine.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TPaveText.h>

Int_t idDetMap[160] = {60,-1,-1,-1,-1,-1,-1,-1,-1,-1,//RF TIMING STUFF
		       40,41,42,43,44,45,46,47,-1,-1,//ELUM
		       48,49,50,51,52,53,54,55,-1,-1,//ELUM
		       30,30,31,31,32,32,33,33,-1,-1,//Recoil Pairs have same detid
		       1,0,5,4,3,2,1,0,-1,-1,/*1*/
		       3,2,1,0,5,4,3,2,-1,-1,/*2*/
		       11,10,9,8,7,6,5,4,-1,-1,/*3*/
		       -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,//Empty
		       7,6,11,10,9,8,7,6,-1,-1,/*4*/
		       15,14,13,12,11,10,9,8,-1,-1,/*5*/
		       17,16,15,14,13,12,17,16,-1,-1,/*6*/
		       -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,//Empty
		       19,18,17,16,15,14,13,12,-1,-1,/*7*/
		       21,20,19,18,23,22,21,20,-1,-1,/*8*/
		       23,22,21,20,19,18,23,22,-1,-1,/*9*/      
		       -1,-2,-3,-4,-5,-6,-7,-8,-9,-10};///
Int_t idKindMap[160] = {6,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			5,5,5,5,5,5,5,5,-1,-1,
			5,5,5,5,5,5,5,5,-1,-1,
			3,4,3,4,3,4,3,4,-1,-1,//3=DE,4=E
			1,1,0,0,0,0,0,0,-1,-1,//1 0==energy 1==xf 2==xn
			2,2,2,2,1,1,1,1,-1,-1,//2
			0,0,0,0,0,0,2,2,-1,-1,//3
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			2,2,1,1,1,1,1,1,-1,-1,//4
			0,0,0,0,2,2,2,2,-1,-1,//5
			2,2,2,2,2,2,0,0,-1,-1,//6
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			0,0,1,1,1,1,1,1,-1,-1,//7
			1,1,1,1,0,0,0,0,-1,-1,//8
			2,2,2,2,2,2,1,1,-1,-1,//9
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
//TFile *oFile;
TTree *gen_tree;
Bool_t traceprocessing=1;
Long_t garbage=0;
Long_t tac=0;
Long_t good=0;
Long_t arr=0;
Long_t recoil=0;
Long_t elum=0;
Long_t total=0;
Long_t dropped=0;
Double_t total_chi2=0;
Long_t n_chi2=0;
TH3I *hmult;
Long_t filled=0;
TH1F *htrace;
TH1F *htrig;
TH1F *hint;
TGraph *tg1;
TGraph *tg2;
TLine *line1;
TLine *line2;
TF1 *f1;
TCanvas *ct0;
TPaveText *pt;
typedef struct {
  
  Int_t EventType;
  
  Float_t Energy[24];
  ULong64_t EnergyTimestamp[24];
  Float_t ArrayRise[24];
  Float_t ETC[24];

  Float_t XF[24];
  ULong64_t XFTimestamp[24];

  Float_t XN[24];
  ULong64_t XNTimestamp[24];

  Float_t RDT[8];
  ULong64_t RDTTimestamp[8];
  Float_t RDTRise[8];
  Float_t RTC[8];

  Float_t ELUM[16];
  ULong64_t ELUMTimestamp[16];
  
  Float_t TAC[10];
  ULong64_t TACTimestamp[10];
  Float_t TACTC[10];
   
  Float_t EZERO[4];//0,1 - DE0,E0  
  ULong64_t EZEROTimestamp[4];

} OUTPUT;
OUTPUT psd;

Int_t type(Int_t detkind[],Int_t detid[],Int_t nhits)
{
  // traceprocessing=0;
  Int_t et=-1;
  Int_t narray=0;
  Int_t nrecoil=0;
  Int_t ntac=0;
  Int_t nelum=0;
  Int_t nxf;
  Int_t nxn;
  Int_t ne;
  Int_t nde;
  Int_t nre;
  Int_t gooddets[nhits];
  for(Int_t i=0;i<nhits;i++){
    gooddets[i]=-1;
  }
  for(Int_t i=0;i<nhits;i++){
    ne=0;
    nxf=0;
    nxn=0;
    nde=0;
    nre=0;
    if(detkind[i]==0){
      ne++;
      for(Int_t j=0;j<nhits;j++){
	if(detkind[j]==0&&j!=i&&detid[j]==detid[i]) ne++;
	if(ne>1) break;
	if(detkind[j]==1&&detid[j]==detid[i]) nxf++;
	if(nxf>1) break;
	if(detkind[j]==2&&detid[j]==detid[i]) nxn++;
	if(nxn>1) break;
      }
      if(ne==1&&(nxf==1||nxn==1)&&nxf<2&&nxn<2){
	narray++;
	gooddets[i]=detid[i];
      }
    
    }
    if(detkind[i]==3){
      nde++;
      for(Int_t j=0;j<nhits;j++){
	if(detkind[j]==3&&j!=i&&detid[i]==detid[j]) nde++;
	if(nde>1) break;
	if(detkind[j]==4&&detid[i]==detid[j]) nre++;
	if(nre>1) break;
      }
      if(nde==1&&nre==1){
	nrecoil++;
	gooddets[i]=detid[i];
      }
  
    }
    if(detkind[i]==5){
      nelum++;
      gooddets[i]=detid[i];
    }
    if(detkind[i]==6){
      ntac++;
      gooddets[i]=detid[i];
    }
  }
  for(Int_t i=0;i<nhits;i++){
    Bool_t isgood=0;
    for(Int_t j=0;j<nhits;j++){
      if(detid[i]==gooddets[j]) isgood=1;
    }
    //  if(!isgood) detkind[i]=-1;
  }
  Bool_t r=0;
  Bool_t e=0;
  Bool_t a=0;
  Bool_t t=0;
  if(nrecoil>0){
    recoil++;
    r=1;
  }
  if(narray>0){
    arr++;
    a=1;
  }
  if(nelum>0){
    elum++;
    e=1;
  }
  if(ntac>0){
    tac++;
    t=1;
  }
  if(!a&&!r&&!t&&!e) et=0;
  if(!a&&!r&&!t&&e) et=1;
  if(!a&&!r&&t&&!e) et=2;
  if(!a&&!r&&t&&e) et=3;
  if(!a&&r&&!t&&!e) et=4;
  if(!a&&r&&!t&&e) et=5;
  if(!a&&r&&t&&!e) et=6;
  if(!a&&r&&t&&e) et=7;
  if(a&&!r&&!t&&!e) et=8;
  if(a&&!r&&!t&&e) et=9;
  if(a&&!r&&t&&!e) et=10;
  if(a&&!r&&t&&e) et=11;
  if(a&&r&&!t&&!e) et=12;
  if(a&&r&&!t&&e) et=13;
  if(a&&r&&t&&!e) et=14;
  if(a&&r&&t&&e) et=15;
  
  return et;
}

void builder::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  //oFile=new TFile("gen.root","RECREATE");
  TString option = GetOption();

}

void builder::SlaveBegin(TTree * /*tree*/)
{
 
  f1=new TF1("f1","pol4",0,203);
  hmult=new TH3I("hmult","Time difference vs channel vs channel",61,0,61,61,0,61,1000,0,1000);
  fOutput->Add(hmult);
  htrace=new TH1F("htrace","trace",203,0,203);
  htrig=new TH1F("htrig","trigger",203,0,203);
  hint=new TH1F("hint","integral",203,0,203);
  ct0=new TCanvas("ct0");
  pt=new TPaveText(0.8,0.8,1,1,"brNDC");
    
  gen_tree = new TTree("gen_tree","PSD Tree");
  gen_tree->Branch("event_type",&psd.EventType);
  gen_tree->Branch("e",psd.Energy,"Energy[24]/F");
  gen_tree->Branch("e_t",psd.EnergyTimestamp,"EnergyTimestamp[24]/l");
  gen_tree->Branch("e_tc",psd.ETC,"EnergyTimestampCorrection[24]/F");
  gen_tree->Branch("a_rise",psd.ArrayRise,"ArrayRise[24]/F");
 
  gen_tree->Branch("xf",psd.XF,"XF[24]/F");
  gen_tree->Branch("xf_t",psd.XFTimestamp,"XFTimestamp[24]/l");
 
  gen_tree->Branch("xn",psd.XN,"XN[24]/F");
  gen_tree->Branch("xn_t",psd.XNTimestamp,"XNTimestamp[24]/l"); 
 
  gen_tree->Branch("rdt",psd.RDT,"RDT[8]/F");
  gen_tree->Branch("rdt_t",psd.RDTTimestamp,"RDTTimestamp[8]/l"); 
  gen_tree->Branch("rdt_tc",psd.RTC,"RDTTimestampCorrection[8]/F");
  gen_tree->Branch("rdt_rise",psd.RDTRise,"RDTRise[8]/F");	   

  gen_tree->Branch("tac",psd.TAC,"TAC[10]/F");
  gen_tree->Branch("tac_t",psd.TACTimestamp,"TACTimestamp/l"); 
  
  gen_tree->Branch("elum",psd.ELUM,"ELUM[16]/F");
  gen_tree->Branch("elum_t",psd.ELUMTimestamp,"ELUMTimestamp[16]/l");
 
  gen_tree->Branch("ezero",psd.EZERO,"EZERO[4]/F");
  gen_tree->Branch("ezero_t",psd.EZEROTimestamp,"EZEROTimestamp[4]/l"); 
  fOutput->Add(gen_tree);
  
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

}

Bool_t builder::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either builder::GetEntry() or TBranch::GetEntry()
  // to read either all or the required parts of the data. When processing
  // keyed objects with PROOF, the object is already loaded and is available
  // via the fObject pointer.
  //
  // This function should contain the "body" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.
  //zero struct
  for (Int_t i=0;i<24;i++){//num dets
    psd.Energy[i]=0;
    psd.XF[i]=0;
    psd.XN[i]=0;
    psd.EnergyTimestamp[i]=0;
    psd.ETC[i]=0;
    psd.XFTimestamp[i]=0;
    psd.XNTimestamp[i]=0;
    psd.ArrayRise[i]=0;
    if (i<16){
      psd.ELUM[i]=0;
      psd.ELUMTimestamp[i]=0;
    }
    if (i<8){
      psd.RDT[i]=0;
      psd.RDTTimestamp[i]=0;
      psd.RDTRise[i]=0;
      psd.RTC[i]=0;
    }
    if (i==0){
      psd.TAC[0]=0;
      psd.EventType=0;
    }
  }

 
  b_NumHits->GetEntry(entry);
  b_id->GetEntry(entry);
  b_event_timestamp->GetEntry(entry);
  b_pre_rise_energy->GetEntry(entry);
  b_post_rise_energy->GetEntry(entry);
  b_trace_length->GetEntry(entry);
 
  total++; 
  Int_t nhits=NumHits;
  Int_t detkind[nhits];
  Int_t detid[nhits];
  Float_t energy[nhits];
  for(Int_t i=0;i<nhits;i++){
    detkind[i]=-1;
    detid[i]=-1;
  }
  for(Int_t i=0;i<nhits;i++){
    Int_t chan=id[i]-1010;
    detkind[i]=idKindMap[chan];
    detid[i]=idDetMap[chan];
    energy[i]=(post_rise_energy[i]-pre_rise_energy[i])/100;
  }


  for(Int_t i=0;i<nhits;i++){
 
    if((detkind[i]==0||detkind[i]==1||detkind[i]==2||detkind[i]==5||detkind[i]==6)&&energy[i]<0) detkind[i]=-1;
    if((detkind[i]==3||detkind[i]==4)&&energy[i]>0) detkind[i]=-1;
  }
  Int_t timeij;
  if(nhits>1){
    for(Int_t i=1;i<nhits;i++){
      for(Int_t j=0;j<i;j++){
  	timeij=event_timestamp[i]-event_timestamp[j];
  	if(detkind[i]!=6){
  	  if(timeij>200) detkind[i]=-1;
  	}
  	if(detkind[i]==6){
  	  if(timeij>693||timeij<640) detkind[i]=-1;
  	}
      }
    }
  }
  Int_t et1=-1;
  Int_t et2=-1;
  Int_t et3=-1;
  Int_t et4=-1;
  Int_t et5=-1;
  et1=type(detkind,detid,nhits);
  ///////////////////////////TRACE PROCESSING/////////////////////////////////////
  if(traceprocessing){
    b_trace->GetEntry(entry);
    b_m2_end_sample->GetEntry(entry);
    for(Int_t i=0;i<nhits;i++){
      if(detkind[i]>-1&&TMath::Abs(energy[i])>0){

	Int_t watchtrace=999;
	Bool_t saturated=0;
	Bool_t pileup=0;
	Float_t norm[203];
	Double_t chi2=0;
	///////////////SATURATION DETECT////////////////////////////////
	Int_t nsat=0;
	for(Int_t j=0;j<203;j++){
	  if((trace[i][j]&0x3fff)==0||(trace[i][j]&0x3fff)==1||(trace[i][j]&0x3fff)==16384||(trace[i][j]&0x3fff)==16383) nsat++;
	  if(nsat>10){
	    saturated=1;
	    detkind[i]=-1;
	    et2=type(detkind,detid,nhits);
	    break;
	  }
	}
	/////////////////END SATURATION DETECT////////////////////////
	///////////////////BEGIN SEMI-GAUSSIAN FILTER///////////////////////
	if(traceprocessing&&detkind[i]>-1){
	  Float_t diff[220];
	  Float_t trigger[220];
	  Float_t integral[220];
	  Int_t n=203;
	  Int_t n_poles=6;
	  Int_t nt_poles=6;
	  Double_t tc=5;
	  Double_t tc1=2;
	  if(detkind[i]==3) tc1=1;
	  if(detkind[i]==0) tc1=1;
	  Float_t pz=44000;
	  if(detkind[i]==4) pz=80000;
	  Float_t gain=8.3; //gain=1/(n_poles^n_poles*exp(-n_poles)/n_poles!)
	  Float_t gaint= 14.5;
	 
	  ///////////////Energy Shaping time/////////////////
	  Double_t b1=TMath::Exp(-1.0/tc);
	  Double_t a0=(1.0+b1)/2.0;
	  Double_t a1=-(1.0+b1)/2.0;
	  diff[0]=0;
	  for(Int_t j=1;j<n;j++){
	    diff[j]=b1*diff[j-1]+a0*(trace[i][j]&0x3fff)+a1*(trace[i][j-1]&0x3fff)+(trace[i][j-1]&0x3fff)/pz;
	  }
	  if(detkind[i]==3||detkind[i]==4){
	    for(Int_t j=1;j<n;j++){
	      diff[j]*=-1;
	    }
	  }
	  a0=1.0-b1;
	  integral[0]=0;
	  for(Int_t ii=1;ii<n_poles+1;ii++){
	    for(Int_t j=1;j<n;j++){
	      integral[j]=b1*integral[j-1]+a0*diff[j];
	    }
	    for(Int_t j=1;j<n;j++){
	      diff[j]=integral[j];
	    }
	  }
	  for(Int_t ii=1;ii<n;ii++){
	    integral[ii]*=gain;
	  }
	  ////////////Trigger made from Array and DE/////////////
	  b1=TMath::Exp(-1.0/tc1);
	  a0=(1.0+b1)/2.0;
	  a1=-(1.0+b1)/2.0;
	  diff[0]=0;
	  for(Int_t j=1;j<n;j++){
	    diff[j]=b1*diff[j-1]+a0*(trace[i][j]&0x3fff)+a1*(trace[i][j-1]&0x3fff)+(trace[i][j-1]&0x3fff)/pz;
	  }
	  if(detkind[i]==3||detkind[i]==4){
	    for(Int_t j=1;j<n;j++){
	      diff[j]*=-1;
	    }
	  }
	  a0=1.0-b1;
	  trigger[0]=0;
	  for(Int_t ii=1;ii<nt_poles+1;ii++){
	    for(Int_t j=1;j<n;j++){
	      trigger[j]=b1*trigger[j-1]+a0*diff[j];
	    }
	    for(Int_t j=1;j<n;j++){
	      diff[j]=trigger[j];
	    }
	  }
	  for(Int_t ii=1;ii<n;ii++){
	    trigger[ii]*=gaint;
	  }
	  /////////Pulse height info from Integral[203], timing from Trigger[203]
	  ////////////////////finish semi-gaussian filter///////////////////////
	  
	  ///////////////////////////PEAK DETECT///////////////////////////////
	  Int_t npeaks=0;
	  Int_t ntpeaks=0;
	  Double_t xpeaks[100];
	  Double_t ypeaks[100];
	  Double_t xtpeaks[100];
	  Double_t ytpeaks[100];
	  for(Int_t j=0;j<100;j++){
	    xpeaks[j]=0;
	    ypeaks[j]=0;
	    xtpeaks[j]=0;
	    ytpeaks[j]=0;
	  }
	  Int_t threshold=500;
	  if(detkind[i]==0||detkind[i]==5||detkind[i]==6||detkind[i]==4) threshold=90;
	  if(detkind[i]==1||detkind[i]==2||detkind[i]==3) threshold=90;
	 
	  //take care of pileup detect in trigger: more sensitive//
	  for(Int_t ii=5;ii<n+5;ii++){
	    if(integral[ii]>threshold&&(integral[ii]>integral[ii-1]&&integral[ii]>integral[ii-2]&&integral[ii]>integral[ii-3]&&integral[ii]>integral[ii-4]&&integral[ii]>integral[ii-5])&&
	       (integral[ii]>integral[ii+1]&&integral[ii]>integral[ii+2]&&integral[ii]>integral[ii+3]&&integral[ii]>integral[ii+4]&&integral[ii]>integral[ii+5])){
	      xpeaks[npeaks]=ii;
	      ypeaks[npeaks]=integral[ii];
	      if(xpeaks[npeaks]<203)   npeaks++;
	   
	    }
	    if(trigger[ii]>threshold&&(trigger[ii]>trigger[ii-1]&&trigger[ii]>trigger[ii-2]&&trigger[ii]>trigger[ii-3]&&trigger[ii]>trigger[ii-4]&&trigger[ii]>trigger[ii-5])&&
	       (trigger[ii]>trigger[ii+1]&&trigger[ii]>trigger[ii+2]&&trigger[ii]>trigger[ii+3]&&trigger[ii]>trigger[ii+4]&&trigger[ii]>trigger[ii+5])){
	      xtpeaks[ntpeaks]=ii;
	      ytpeaks[ntpeaks]=trigger[ii];
	      if(xtpeaks[ntpeaks]<203) ntpeaks++;
	      if(detkind[i]==3&&ntpeaks>1){
		pileup=1;
		//	detkind[i]=-1;//comment out to see pileup
		watchtrace=detkind[i]; //uncomment to see pileup
		et3=type(detkind,detid,nhits);
	      }
	    }
	  }

	  // if(npeaks==0) detkind[i]=-1;
	  et4=type(detkind,detid,nhits);

	  ////////////////////END PEAK DETECT////////////////////////////
	  //////////////////BEGIN TIMING DETERMINATION///////////////////
	  if(traceprocessing&&detkind[i]>-1){
	    Int_t start=0;
	    Int_t rangelookup=0;
	    for(Int_t j=0;j<203;j++){
	      norm[j]=((Float_t)(trace[i][j]&0x3fff)-(Float_t)m2_end_sample[i])*8000/energy[i];
	      // htrace->SetBinContent(j,trace[i][j]&0x3fff);
	      htrace->SetBinContent(j,norm[j]);
	      if(norm[j]>2000&&start==0) start=j-5;
	      if(norm[j]>7500&&rangelookup==0) rangelookup=j;
	    }
	  
	    for(Int_t j=1;j<202;j++){
	       if(trigger[j]>threshold&&start==0) start=j-3;
	      htrig->SetBinContent(j,trigger[j]);
	      hint->SetBinContent(j,integral[j]);
	    }
	   	  
	    TFitResultPtr r;
	    TFitResultPtr s;
	    Int_t fitstatus=-1;
	    Int_t fitstatuss=-999;
	    Int_t fitstatusr=-999;
	    Float_t rise=0;
	    Float_t tc=0;
	    if(detkind[i]==0||detkind[i]==3||detkind[i]==4){
	      f1->SetRange(start,rangelookup);
	      if(ntpeaks>0){
		r=htrace->Fit("f1","NRQ");
		fitstatus=r;
		if(fitstatus!=0){
		  s=htrace->Fit("f1","NMQR");
		  fitstatus=s;
		  if(fitstatus-4000!=0){
		    cout<<"R Error "<< fitstatusr<< " S Error "<<fitstatuss<< " could not fit"<< endl;
		  }
		}
	      }
	      rise=f1->GetX(7200)-f1->GetX(800);
	      tc=f1->GetX(800);
	    }
	    if(watchtrace==detkind[i]||watchtrace==999){
	      pt->Clear();
	      TString text("Detector type ");
	      text+=detkind[i];
	      pt->AddText(text);
	      TString text2("Detector id ");
	      text2+=detid[i];
	      pt->AddText(text2);
	      tg1=new TGraph(ntpeaks,xtpeaks,ytpeaks);
	      tg2=new TGraph(npeaks,xpeaks,ypeaks);
	      tg1->SetLineColor(6);
	      tg2->SetLineColor(2);
	      line1=new TLine(tc,-100,tc,12000);
	      line2=new TLine(rangelookup,-100,rangelookup,12000);
	      line1->SetLineStyle(10);
	      line2->SetLineStyle(10);
	      hint->SetAxisRange(-100,12000,"Y");
	      hint->SetLineColor(2);
	      hint->SetStats(0);
	      hint->Draw();
	      htrig->SetLineColor(6);
	      htrig->Draw("same");
	      htrace->Draw("same");
	      f1->SetLineColor(4);
	      f1->Draw("same");
	      tg1->Draw("C*");
	      tg2->Draw("C*");
	      line1->Draw();
	      line2->Draw();
	      pt->Draw();
	      ct0->Update();
	      cout<<ntpeaks<<" trigger peaks and "<<npeaks<<" energy peaks at "<<xpeaks[0]<<endl;
	      gPad->WaitPrimitive();
	      watchtrace=-999;
	      tg1->Delete();
	      tg2->Delete();
	    }//watchtrace options.

	    //////////////////////END TIMING///////////////////////////////////
	    /////////////Fill output structure with EVERYTHING/////////////////
	    switch(detkind[i]){
	    case 0:
	      psd.Energy[detid[i]]=energy[i];
	      psd.EnergyTimestamp[detid[i]]=event_timestamp[i];
	      psd.ArrayRise[detid[i]]=rise;
	      psd.ETC[detid[i]]=tc;
	      break;
	    case 1:
	      psd.XF[detid[i]]=energy[i];
	      psd.XFTimestamp[detid[i]]=event_timestamp[i];
	      break;
	    case 2:
	      psd.XN[detid[i]]=energy[i];
	      psd.XNTimestamp[detid[i]]=event_timestamp[i];
	      break;
	    case 3:
	      psd.RDT[(detid[i]-30)*2]=TMath::Abs(energy[i]);
	      psd.RDTTimestamp[(detid[i]-30)*2]=event_timestamp[i];
	      psd.RDTRise[(detid[i]-30)*2]=rise;
	      psd.RTC[(detid[i]-30)*2]=tc;
	      break;
	    case 4:
	       psd.RDT[(detid[i]-30)*2+1]=TMath::Abs(energy[i]);
	      //psd.RDT[(detid[i]-30)*2+1]=ypeaks[0];
	      psd.RDTTimestamp[(detid[i]-30)*2+1]=event_timestamp[i];
	      psd.RDTRise[(detid[i]-30)*2+1]=rise;
	      psd.RTC[(detid[i]-30)*2+1]=tc;
	      break;
	    case 5:
	      psd.ELUM[detid[i]-40]=energy[i];
	      psd.ELUMTimestamp[detid[i]-40]=event_timestamp[i];
	      break;
	    case 6:
	      psd.TAC[0]=energy[i];
	      psd.TACTimestamp[0]=event_timestamp[i];
	      break;
	    }
	    //////////////////////END FILL OUTPUT STRUCTURE//////////////////
	  }//After peak detect. Third event type determination and pilup rejection. Can fill structure before here, after rise and etc determination.
	} //After saturation test, determine that event type is still good and detkind!=-1. Second if(traceprocssing&&detkind!+=1)
      }//first if(detkind[i]>-1) condition
    } //end NumHits loop
  }//end first if(traceprocessing) condition
  gen_tree->Fill();

  ////////////////////////////END TRACE PROCESSING//////////////////////
  et5=type(detkind,detid,nhits);
  psd.EventType=et5;
  if(traceprocessing) good++;
  else garbage++;
  
  Bool_t fill=0;
  if(et1!=et5&&et1>9) dropped++;
  if(traceprocessing) fill=1;
 
 
 if(fill){	  
    for(Int_t i=1;i<nhits;i++){
      for(Int_t j=0;j<i;j++){
	if(detkind[i]!=-1&&detkind[j]!=-1){
	  hmult->Fill(idDetMap[id[i]-1010],idDetMap[id[j]-1010],event_timestamp[i]-event_timestamp[j]);
	}
      }
    }
  }//hmult fill
  return kTRUE;
} //end Process

void builder::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

}

void builder::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  // oFile->cd();
  // cout<<"looking for hmult"<<endl;
  // hmult=dynamic_cast<TH3I*>(fOutput->FindObject("hmult"));
  // if(hmult){
  //   cout<<"hmult found"<<endl;
  //   hmult->Write();
  //   cout<<"hmult written"<<endl;
  // // hmult->Draw();
  // }
  // gen_tree= dynamic_cast<TTree*>(fOutput->FindObject("gen_tree"));
  // if(gen_tree){
  //   cout<<"gen_tree found, attempting to write"<<endl;
  //   gen_tree->Write();
  //   cout<<"Tree found and succesfully written"<<endl;
  // }
  // hmult=dynamic_cast<TH3I*>(fOutput->FindObject("hmult"));
  // if(hmult) hmult->Write();
  // oFile->Close();
}
