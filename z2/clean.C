#define clean_cxx
// The class definition in clean.h has been generated automatically
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
// root> T->Process("clean.C")
// root> T->Process("clean.C","some options")
// root> T->Process("clean.C+")
//

#include "clean.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <TH3.h>
#include <TMath.h>


TFile *oFile;
TH1F *hf[9];
TH1F *hfr[9];
TH2F *hevx[24];
TH2F *hez[24];
TH2F *hezc[24];
TH2F *hxfxn[24];
TH2F *hesum[24];
TH2F *hexc[24];
TH1F *hrtac2[4][24];
TH2F *hrve[4];
TH2F *hrvec[4];
TH2F *htve[24];
TH2F *hxect;
TH2F *hxfxnc[24];
TH2F *hesumc[24];
TH2F *htx[24];
TH2F *hezg[9];
TH2F *hezg2[4][24];
//TH3F *hetz[4];
TH2F *hez_all;
TH2F *hezs[4];
TH2F *hxtac[24];
TH2F *hr[4];
TH2F *hrg[4];
TH2F *hrtac[4];
TH2F *hrtacg;
ULong64_t add;
ULong64_t NumEntries = 0;
ULong64_t ProcessedEntries = 0;
Float_t Frac = 0.1;
TCutG *na2;

typedef struct{
  
  Float_t e;
  Float_t xf;
  Float_t xn;
  Float_t de;
  Float_t re;
  Float_t tac;
  Float_t x;
  Float_t z;
  
}data;

typedef struct{
  Long64_t e;
  Long64_t xf;
  Long64_t xn;
  Long64_t de;
  Long64_t tac;
  
  Float_t etc;
  Float_t detc;

}timing;

// Float_t xcoef[24] = {0.949029,1.01802,0.971783,0.927059,0.991241,0.881107,
// 		     0.906056,0.926736,0.989037, 1.00974,0.950697, 0,
// 		     1.03673,1.00772,1.00793,0.970457,0.955211,1.12365,
// 		     0.965497,0.977822, 0.889425,0.983675,1.08844,1.09201};
Float_t xcoef[24] = {0.94,0.99,0.97,0.93,1.045,0.95,
		     0.91,.98,1.01, 0.95,0.99, 0,
		     0.98,1.02,1.00793,0.99,1.01,0.93,
		     0.965497,1.0, 0.92,0.985,1.05,1.08};
Float_t xstretch[24]={1,1,1,1,1,1.01,
		      0.81,0.87,0.82,1,1.01,1,
		      0.83,0.81,0.85,0.83,1,1,
		      1,1,1,1,1,1};
  Float_t xcenter[24]={0,0,0.01,0,0,0,
		       0.07,0.07,0.1,0,0,0,
		       0.07,0.08,0.06,0.1,0,0,
		       0,0,0,0,0,0};
Float_t ecoef[24] = {0.9,0.9, 1.05,1.09,.852,0.95,
		     1.05,0.955,0.95,0.97,0.927987,0,
		     0.9,0.92,0.922803,0.96689,1.0,.975,
		     0.896227,0.958843,1.03,0.885,1.09,0.879};

///xt calibrations////
Float_t p0[24]={-2.869,-2.035,-1.708,-2.162,-2.047,-1.727,
		-2.693,-1.009,-2.522,-2.673,-2.052,0,
		-6.394,-6.356,-5.899,-5.544,-6.339,-6.429,
		-4.229,-4.150,-3.635,-3.978,-3.826,-3.812};
Float_t p1[24]={3.9303,-2.84079,-0.461684,17.9265,28.4461,38.2932,
		2.19152,0.665814,4.47166,25.8992,25.4893,0,
		6.00503,-1.56817,-0.593538,3.49965,-1.00911,-0.832172,
		2.16823,1.65914,-0.83749,2.40862,1.55533,0.424124};
Float_t p2[24]={40.5575,99.8466,81.446,-168.315,-256.496,-335.959,
		10.5271,35.4578,4.10564,-206.82,-174.593,0,
		-5.99183,22.2336,17.3367,9.82315,72.9097,70.865,
		40.3466,47.9315,65.2038,35.212,48.1242,45.2454};
Float_t p3[24]={-89.3624,-189.934,-160.03,1095.22,1375.2,1668.16,
		-23.2612,-68.676,-15.5407,1104.57,876.262,0,
		0,-32.1538,-31.8695,-26.683,-143.71,-139.787,
		-86.1288,-100.492,-126.583,-74.8577,-100.214,-90.8535};
Float_t p4[24]={45.0386,92.7441,79.052,-2528.57,-2930.57,-3436.76,
		10.8737,32.682,6.98687,-2388.58,-1851.18,0,
		0,12.3621,14.7049,13.2753,71.6839,69.8255,
		43.6401,50.9485,62.2398,37.2459,50.496,45.0125};
Float_t p5[24]={0,0,0,2376.91,2638.12,3048.28,
		0,0,0,2182.14,1660.27,0,
		0,0,0,0,0,0,
		0,0,0,0,0,0};
Float_t p6[24]={0,0,0,-792.989,-854.609,-982.011,
		0,0,0,-716.983,-536.041,0,
		0,0,0,0,0,0,
		0,0,0,0,0,0};
///////////////////////////

/////e calibrations///////




/////////////////////////////////////////

Float_t ep0[24]={0.514816,0.460846,0.596502,0.548282,0.600,0.4,
		 0.60414,0.651181,0.53368,0.289059,0.813,0,
		 0.603175,0.668597,0.509103,0.60944,1.03,0.52,
		 0.62862,0.539504,0.661149,0.409447,0.48,0.43};
Float_t ep1[24]={0.00361954,0.00391184,0.00384472,0.00419951,0.0035765,0.00412867,
		 0.00406082,0.00381772,0.00385497,0.00400755,0.00373899,0,
		 0.0039205,0.00374873,0.00397163,0.00389666,0.00345367,0.00387393,
		 0.00363259,0.0038533,0.00394617,0.0037813,0.00387696,0.00388486};
// Float_t ep0[24]={0.110497,-0.03,0.213181,0.04,0.3,0.56,
// 		 0.42932,0.15,0,0.27,0.40,0,
// 		 0.547691,0.45,0.3013,0.256,-0.05,-0.4,
// 		 0.244061,0.2,0.201385,0.136,-0.15,-0.3};
// Float_t ep1[24]={0.00344027,0.00382559,0.00369782,0.00415,0.00335373,0.00349896,
// 		 0.00366834,0.00368933,0.00375056,0.0035,0.00348784,0,
// 		 0.00343868,0.00345896,0.00359138,0.00365,0.00389493,0.0045,
// 		 0.00350118,0.00360443,0.00375562,0.00355,0.00400049,0.00421561};
Float_t ep2[24]={-32.251,6.9,39.9,-6.9,11.789,39.6482,
		 36.2922,19.542,23.503,14.6892,23.4317,0,
		 13.3247,-34.0011,13.9187,-15.8526,21.0526,22.4894,
		 31.6063,-33.7536,-23.5623,38.3936,-11.2209,-6.93722};
Float_t ep3[24]={0,0,0,0,0,0,
		 0,0,0,0,0,0,
		 0,0,0,0,0,0,
		 0,0,0,0,0,0};
////recoil rise time corrections///////
// E0 fit from 35 to 100
Float_t re0[4]={-10915.6,-173069,-11024,-409.585};
Float_t re1[4]={1132.01,9483.24,1431.41,523.745};
Float_t re2[4]={-37.822,-193.364,-60.291,-18.9847};
Float_t re3[4]={0.586342,1.8904,1.17411,0.262759};
Float_t re4[4]={-0.00435744,-0.0089758,-0.0109122,-0.00161543};
Float_t re5[4]={0.0000125976,0.0000166797,0.0000392694,0.00000370862};

//////////////////////////////////////////////////////////////////

Float_t active=50.5; //Length of active area in mm
Int_t offset=-500; //Distance in mm between active detector area and target
Float_t positions[7]={offset-active/2+positions[1],
		      66.76,125.76,184.76,243.66,302.96,361.56};
data raw;
data cal;
timing t;
data corr;

ULong64_t processed=0;
ULong64_t faults=0;
TStopwatch StpWatch;
void clean::Begin(TTree *tree)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
  NumEntries = tree->GetEntries();
  TString option = GetOption();
  StpWatch.Start();
 
}

void clean::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).
  for(Int_t i=0;i<24;i++){
    hxfxn[i]=new TH2F(Form("hxfxn%d",i),Form("xf vs xn for det %i",i),512,0,4000,512,0,4000);
    hevx[i]=new TH2F(Form("hevx%d",i),Form("e vs x for det %i",i),512,-2,2,512,0,4000);
    hez[i]=new TH2F(Form("hez%d",i),Form("e vs z for det %i",i),1024,-900,0,512,0,4000);
    hezc[i]=new TH2F(Form("hezc%d",i),Form("e vs z corrected for det %i",i),1024,-900,0,5120,0,12);
    htx[i]=new TH2F(Form("htx%d",i),Form("t vs x for det %i",i),512,-2,2,512,-50,50);
    hesum[i]=new TH2F(Form("hesum%d",i),Form("e vs xf+xn for det %i",i),512,0,4000,512,0,4000);
    hxfxnc[i]=new TH2F(Form("hxfxnc%d",i),Form("xf vs xn for det %i",i),512,0,4000,512,0,4000);
    hexc[i]=new TH2F(Form("hexc%d",i),Form("e vs x for det %i",i),512,-2,2,512,0,12);
    hesumc[i]=new TH2F(Form("hesumc%d",i),Form("e vs xf+xn for det %i",i),512,0,4000,512,0,4000);
    hxtac[i]=new TH2F(Form("hxtac%d",i),Form("tac vs x gated on time difference for det %i",i),500,-0.5,1.5,512,0,4000);
    htve[i]=new TH2F(Form("htve%d",i),Form("timestamp vs entry%d",i),1000,0,10000,1000,0,10000);
  }
 
   for(Int_t i=0;i<9;i++){
  hezg[i]=new TH2F(Form("hezg%d",i),Form("gated e vs z for turn ID# %i",i),512,-1000,0,512,0,12);
  hf[i]=new TH1F(Form("hf%d",i),Form("excitation energy for tac peak %d",i),512,-15,15);
  hfr[i]=new TH1F(Form("hfr%d",i),Form("excitation energy for random tac peak %d",i),512,-15,15);

  }
  hez_all=new TH2F("hez_all","e vs z ungated",1024,-1000,0,512,0,12);
 
 
 
  hrtacg=new TH2F("hrtacg","gated and summed recoil tac",1024,-200,200,24,0,24);
  hxect=new TH2F("hxect"," test of energy calibration",1024,-0.1,1.1,10024,-1,1400);
  for(Int_t i=0;i<4;i++){
    hrtac[i]=new TH2F(Form("hrtac%d",i),Form("recoil tac vs array channel for recoil id%d",i),1024,-200,200,24,0,24);
    hrve[i]=new TH2F(Form("hrve%d",i),Form("Recoil rise time %d vs energy ",i),1000,0,200,5000,0,5000);
    hrvec[i]=new TH2F(Form("hrvec%d",i),Form("Recoil rise time %d vs energy (corrected)",i),1000,0,200,5000,0,5000);
    hr[i]=new TH2F(Form("hr%d",i),Form("Recoil DE vs E recoil %d",i),512,0,8000,512,0,8000);
    hrg[i]=new TH2F(Form("hrg%d",i),Form("Recoil DE vs E recoil gated %d",i),512,0,8000,512,0,8000);
    hezs[i]=new TH2F(Form("hezs%d",i),Form("E vs Z for side %d",i),1024,-1000,0,512,-4,12);
    //  hetz[i]=new TH3F(Form("het%d",i),Form("E vs time_rel vs z for side %d",i),200,-10,10,20,2,4,150,-540,-490);
    for(Int_t j=0;j<24;j++){
      hrtac2[i][j]=new TH1F(Form("hrtac2%d%d",i,j),Form("recoil-array tac spectrum for rid %d, eid %d",i,j),1024,-200,200);
      hezg2[i][j]=new TH2F(Form("hezg2%d%d",i,j),Form("gated e vs z for side %d recoil %d",i,j),512,-1000,0,512,0,12);
	     }
  }
  TFile *cuts=new TFile("cuts.root");
  na2=(TCutG*)cuts->Get("naccept2");
  if(na2) cout<<"found cut2"<<endl;
  
   TString option = GetOption();

}

Bool_t clean::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either clean::GetEntry() or TBranch::GetEntry()
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


  /////////////Perform array functions///////////////
  ProcessedEntries++;

  if (ProcessedEntries>NumEntries*Frac-1) {
    printf(" %3.0f%% (%llu/%llu Mil) processed in %6.1f seconds\n",Frac*100,ProcessedEntries/1000000,NumEntries/1000000,StpWatch.RealTime());
    StpWatch.Start(kFALSE);
    Frac+=0.1;
  }
    
  b_event_type->GetEntry(entry);
  b_Energy->GetEntry(entry);
  b_XF->GetEntry(entry);
  b_XN->GetEntry(entry);
  b_RDT->GetEntry(entry);
  b_RDTRise->GetEntry(entry);
  b_ArrayRise->GetEntry(entry);

  Int_t eid=-1;
  Int_t xfid=-1;
  Int_t xnid=-1;
  Int_t emult=0;
  Int_t xfmult=0;
  Int_t xnmult=0;

  
  ////////////Array Diagnostic Histograms////////////////
  Float_t rise_thresh[24]={17.9,22.5,19.7,24.4,19.7,24.7,
			  4,12.6,7.3,29.2,24.4,0,
			  3.7,4.2,3.8,6.4,24,26,
			  22,21.8,20.1,16.7,17.2,16.2};
  for(Int_t i=0;i<24;i++){
    if(e[i]>0&&(xf[i]!=0||xn[i]!=0)){
      eid=i;
      emult++;
    }
  }
  if(emult>1) faults++;
  //if(eid>-1&&a_rise[eid]>rise_thresh[eid]){
  if(eid>-1&&emult==1&&a_rise[eid]>rise_thresh[eid]){
    //          if(event_type==14||event_type==15){
    raw.e=e[eid];
    raw.xf=xf[eid];
    raw.xn=xn[eid];
    Float_t xsum=raw.xf+raw.xn;
    Float_t xdiff=raw.xf-raw.xn;
    raw.x=0.5*(1.0+xdiff/xsum);
    //if(raw.x>1.0) cout<<"X value over 1. XF "<<raw.xf<<" XN "<<raw.xn<<endl;
    hxfxn[eid]->Fill(raw.xf,raw.xn);
    hevx[eid]->Fill(raw.x,raw.e);
    hesum[eid]->Fill(xsum,raw.e);
    cal.xn=raw.xn*xcoef[eid];
    //cal.xn=raw.xn;
    cal.xf=raw.xf;
    cal.e=raw.e/ecoef[eid];
    if(cal.xf==0){
      cal.xf=cal.e-cal.xn;
    }
    if(cal.xn==0){
      cal.xn=cal.e-cal.xf;
    }
    xsum=cal.xf+cal.xn;
    xdiff=cal.xf-cal.xn;
    cal.x=0.5*(1.0+xdiff/xsum);
    cal.x/=xstretch[eid];
    cal.x-=xcenter[eid];
    hxfxnc[eid]->Fill(cal.xf,cal.xn);
   
    hesumc[eid]->Fill(xsum,cal.e);
    cal.z=-positions[6-eid%6]-active/2.+positions[0]+(active*cal.x);
    corr.e=cal.e;

    //corr.e+=ep0[eid];
    // Float_t ezcorr=ep1[eid]*cal.x+ep2[eid]*pow(cal.x,2)+ep3[eid]*pow(cal.x,3);
    Float_t ezcorr=ep2[eid]*pow(cal.x,2)+ep3[eid]*pow(cal.x,3);
    // cout<<ezcorr<<endl;
    // corr.e-=ezcorr;
    corr.e*=ep1[eid];
    corr.e+=ep0[eid];
     //corr.e+=0.634*cal.x;
      //  corr.e-=bcoef[eid]-1.4;
    // cout<<cal.z<<endl;
  
    // }

    ///////////////End Array///////////////////////

    ///////////////Recoil Diagnostics//////////////
    Int_t rmult=0;
    Int_t amult=0;






    Int_t rid=-1;
    // Int_t rmult=0;
    
  
    for(Int_t i=0;i<4;i++){
      if(rdt[i*2]>0){
	rid=i;
	rmult++;
      }
    }
    raw.de=rdt[rid*2];
    raw.re=rdt[rid*2+1];
   Float_t fitrangelow[4]={35,86,26,30};
    Float_t fitrangehigh[4]={100,140,80,120};
    Float_t peakhight[4]={1635,2000,1500,3500};
    Float_t rt=rdt_rise[rid*2+1];
    hrve[rid]->Fill(rdt_rise[rid*2+1],raw.re);
    if(rt>fitrangelow[rid]&&rt<fitrangehigh[rid]) cal.re=raw.re*peakhight[rid]/(re0[rid]+re1[rid]*rt+re2[rid]*pow(rt,2)+re3[rid]*pow(rt,3)+re4[rid]*pow(rt,4)+re5[rid]*pow(rt,5));
    else cal.re=0;
    hrvec[rid]->Fill(rdt_rise[rid*2+1],cal.re);
    //if((event_type==1||event_type==2||event_type==3||event_type==4)&&rmult==0) faults++;
    if(rid!=-1) hr[rid]->Fill(cal.re,raw.de);

    
    //////////////////////////////////////////////

    /////////////////Timing///////////////////////
    b_RDTTimestamp->GetEntry(entry);
    b_EnergyTimestamp->GetEntry(entry);
    b_EnergyTimestampCorrection->GetEntry(entry);
    b_RDTTimestampCorrection->GetEntry(entry);
    b_tac->GetEntry(entry);
    b_tac_t->GetEntry(entry);
    Int_t idturn=-999;
    Float_t time_rel=999;
    
      t.e=e_t[eid];
      t.de=rdt_t[rid*2];
      t.etc=e_tc[eid];
      t.detc=rdt_tc[rid*2];
      htve[eid]->Fill(entry,e_t[eid]/10e8);
     
      Float_t time_corr=0;
      //Float_t xtcorr=p0[eid]+p1[eid]*cal.x+p2[eid]*pow(cal.x,2)+p3[eid]*pow(cal.x,3)+p4[eid]*pow(cal.x,4)+p5[eid]*pow(cal.x,5)+p6[eid]*pow(cal.x,6);
      Float_t xtcorr=p1[eid]*cal.x+p2[eid]*pow(cal.x,2)+p3[eid]*pow(cal.x,3)+p4[eid]*pow(cal.x,4)+p5[eid]*pow(cal.x,5)+p6[eid]*pow(cal.x,6);
      time_rel=t.e-t.de;
     
      // hrtac2[rid][eid]->Fill(time_rel);
      time_corr=t.etc-t.detc;
      time_rel+=time_corr;
      time_rel-=xtcorr;
     
      //  time_rel*=10;
      // cout<<t.etc<<" "<<t.detc<<endl;
      if(eid>-1&&rid>-1){
	htx[eid]->Fill(cal.x,time_rel);
	hez_all->Fill(cal.z,corr.e);

	// /////////////////Position 1///////////////////
	// //	if(time_rel>-7.62&&time_rel<-6.05) idturn=0;
	// //	if(time_rel>-3.32&&time_rel<-2.15) idturn=1;
	// if(time_rel>-0.59&&time_rel<2.15) idturn=2;
	// if(time_rel>2.15&&time_rel<4.88) idturn=3;
	// if(time_rel>4.88&&time_rel<7.23) idturn=4;
	// if(time_rel>7.62&&time_rel<10.74) idturn=5;
	// if(time_rel>10.74&&time_rel<13.09) idturn=6;
	// if(time_rel>13.48&&time_rel<15.04) idturn=7;
	// ////////////////////////////////////////////
	//Float_t timeshift[4][4]={{0.626,-0.467,-3.503,0.841},{0.426,-0.558,-3.356,0.668},{3.740,2.562,-0.384,3.884},{1.625,0.513,-2.375,1.817}};
		//	Float_t timeshift[4][24]={{-0.902,-0.605,-0.953,-0.787,-0.794,-0.730,-1.137,-0.939,-0.754,-1.156,-0.889,0,2.144,2.093,2.062,2.298,2.665,2.721,0.040,0.198,0.200,0.147,0.046,0.263},
		//		 {-1.522,-1.365,-1.552,-1.940,-1.918,-1.352,-1.639,-2.127,-1.227,-1.738,-1.602,0,1.646,1.396,1.480,1.518,1.937,1.949,-0.079,-0.197,-0.215,-0.227,-0.524,-0.717},
		//		 {-1.951,-1.786,-2.113,-1.237,-3.307,-3.142,-2.023,-3.753,-2.007,-2.389,-1.379,0,1.531,1.512,1.486,1.704,1.744,1.564,-1.607,-1.169,-1.550,-1.198,-1.704,-0.438},
		//	 	 {-0.767,-0.556,-0.890,-0.724,-0.754,-0.362,-1.250,-0.831,-0.662,-0.856,-1.342,0,2.101,1.739,1.946,1.975,2.527,2.547,0.170,0.575,0.365,0.042,0.345,0.046}};
	 Float_t timeshift[4][24]={{-2.60240e+00,-1.54684e+00,-1.33075e+00,-1.50711e+00,-1.28869e+00,-9.70010e-01,-2.69477e+00 ,-9.41039e-01 ,-1.91239e+00,-2.43824e+00 ,-1.67853e+00 ,0,-3.12264e+00,-3.19758e+00,-2.56692e+00,-1.89280e+00,-2.32044e+00,-2.32137e+00,-2.77009e+00,-2.70964e+00,-2.09231e+00,-2.35235e+00,-2.31421e+00,-2.24468e+00},
	 			 {-3.85428e+00,-2.70382e+00,-2.48971e+00,-2.68997e+00,-2.55831e+00,-2.18765e+00,-3.78571e+00,-1.97816e+00,-3.01783e+00,-3.60676e+00,-2.79658e+00,0,-4.34673e+00,-4.43207e+00,-3.78658e+00,-3.07253e+00,-3.47086e+00,-3.37841e+00,-4.02604e+00,-3.90794e+00,-3.25909e+00,-3.56008e+00,-3.48581e+00,-3.47050e+00},
	 			 {-6.67060e+00,-5.52836e+00,-5.44506e+00,-5.63195e+00,-5.44985e+00,-5.03135e+00,-6.66790e+00,-4.94977e+00,-5.94286e+00,-6.47351e+00,-5.66125e+00,0,-7.27163e+00,-7.42916e+00,-6.72138e+00,-5.94913e+00,-6.29910e+00,-6.24199e+00,-7.03994e+00,-6.89281e+00,-6.28489e+00,-6.44630e+00,-6.51746e+00,-6.44167e+00},
	 			 {-2.50708e+00,-1.42482e+00,-1.18335e+00,-1.45904e+00,-1.21625e+00,-8.70729e-01,-2.54631e+00,-5.97382e-01,-1.73853e+00,-2.25019e+00,-1.54035e+00,0,-2.99861e+00,-3.16459e+00,-2.53619e+00,-1.80750e+00,-2.22701e+00,-2.23673e+00,-2.72280e+00,-2.67713e+00,-2.07873e+00,-2.28491e+00,-2.27736e+00,-2.17577e+00}};


	 // time_rel-=timeshift[side][rid];
	
	  time_rel-=timeshift[rid][eid];
	

	 
	////////////////Position 2//////////////////
	if(time_rel>-0.40&&time_rel<2) idturn=1;
	if(time_rel>1.76&&time_rel<4.10) idturn=2;
	if(time_rel>4.10&&time_rel<6.45) idturn=3;
	if(time_rel>6.84&&time_rel<9.18) idturn=4;
	if(time_rel>10.35&&time_rel<12.30) idturn=5;
	if(time_rel>12.70&&time_rel<14.65) idturn=6;
	//if(time_rel>11.52&&time_rel<13.48) idturn=6;
	//if(time_rel>13.87&&time_rel<15.43) idturn=7;
	Int_t randomid=-1;
	if(time_rel>99.6&&time_rel<101.56){
	  randomid=1;
	  idturn=1;
	}
	if(time_rel>51.66&&time_rel<54){
	  randomid=2;
	  idturn=2;
	}
	if(time_rel>54&&time_rel<56.35){
	  randomid=3;
	  idturn=3;
	}
	//	if(time_rel>6.84&&time_rel<9.18) idturn=4;
	//if(time_rel>10.35&&time_rel<12.30) idturn=5;
	//	if(time_rel>12.70&&time_rel<14.65) idturn=6;

	////////////////////////////////////////////
	Double_t m=938.7820612;
	Double_t c=2.99792458E08;
	Double_t vcm=3.07201E07;
	Double_t vcm2=pow(vcm,2);
	Double_t ecm=corr.e+4.92877-0.0122231*cal.z/idturn;
	Double_t v02=2*ecm/m*pow(c,2);
	Double_t vlab2=2*corr.e/m*pow(c,2);
	Double_t theta=TMath::ACos(vlab2-v02-vcm2/(2*sqrt(v02)*vcm));
    //  cout<<"lab energy "<<corr.e<<" vlab^2 "<<vlab2<<" ecm "<<ecm<<" v02 "<<v02<<" vcm "<<vcm<<endl;
      //      Float_t ex=0.0122915*cal.z+17.774-corr.e;
	Float_t ex=(16.6+6.4650-(29/28)*ecm-0.371)*4.878/5.034;
	Int_t weight =0;
	if(idturn==0) weight=1;
	if(idturn==-1) weight=-1;

	
	if(na2 && !na2->IsInside(cal.z,corr.e)){
	  Int_t side=floor(eid/6);
	  //hezs[side]->Fill(cal.z,corr.e);
	 
	
	
	  Bool_t goodede=0;
	  if(rid==0&&raw.re>0&&raw.de>3000) goodede=1;
	  if(rid==1&&raw.re>0&&raw.de>1300) goodede=1;
	  if(rid==2&&raw.re>0&&raw.de>2500) goodede=1;
	  if(rid==3&&raw.re>0&&raw.de>1450) goodede=1;
	  Bool_t sidecorr=0;
	  // if(rid==1&&side==0) sidecorr=1;
	  // if(rid==0&&side==0) sidecorr=1;
	  // if(rid==0&&side==1) sidecorr=1;
	  // if(rid==3&&side==1) sidecorr=1;
	  // if(rid==3&&side==2) sidecorr=1;
	  // if(rid==2&&side==2) sidecorr=1;
	  // if(rid==2&&side==3) sidecorr=1;
	  // if(rid==1&&side==3) sidecorr=1;
	  if(rid==0&&side==0) sidecorr=1;
	  if(rid==1&&side==3) sidecorr=1;
	  //  if(rid==1&&side==2) sidecorr=1;
	  if(rid==2&&side==2) sidecorr=1;
	  if(rid==3&&side==1) sidecorr=1;
	  if(goodede){
	   hrtac[rid]->Fill(time_rel,eid); 
	   hrg[rid]->Fill(raw.re,raw.de);
	  }
	  if(goodede&&sidecorr){
	    hrtacg->Fill(time_rel,eid);
	    
	    //    hrtac->Fill(time_rel);
	    if(idturn==1) hezg2[side][rid]->Fill(cal.z,corr.e);
	    if(idturn!=-999){ hezg[idturn]->Fill(cal.z,corr.e);
	      hezs[side]->Fill(cal.z,corr.e);
	    
	    }
	     if(idturn>0&&randomid<0) hf[idturn]->Fill(ex);
	     if(randomid>0) hfr[randomid]->Fill(ex);
	     // hf->Fill(ex);
	    //hf->Fill(ex,weight);
	     //  hrtac->Fill(time_rel);
	    hexc[eid]->Fill(cal.x,corr.e);
	    hezc[eid]->Fill(cal.z,corr.e);
	    //  hze[eid]->Fill(cal.z,cal.e);
	    if(eid==0) hxect->Fill(cal.x,corr.e);
	    if(tac_t[0]-t.e>640&&tac_t[0]-t.e<693){
	      hxtac[eid]->Fill(cal.x,tac[0]);
	    }
	  }



	}
      }
      hez[eid]->Fill(cal.z,cal.e);
      Int_t side=floor(eid/6);
     
     
    //////////////////////////////////////////////





    

  }
  return kTRUE;
}

void clean::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void clean::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  // oFile = new TFile("position2.root","RECREATE");
  // TDirectory *hists=oFile->mkdir("hists");
  // hists->cd();
  // TH2F *temp;
  // for(Int_t i=0;i<8;i++){
  // TString name("hezg");
  // name+=i;
  // std::cout<<"looking for "<<name<<std::endl;
  // temp=dynamic_cast<TH2F*>(gROOT->FindObject(name));
  // if(temp) temp->Write();
  // if(!temp) cout<<"couldn't find "<<name<<endl;
  // }
  // oFile->Close();
  std::cout<<processed<<" Events Processed. "<<faults<<" sort3 errors."<<std::endl;
}
