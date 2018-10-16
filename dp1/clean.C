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

TH2F *hevx[24];
TH2F *hxfxn[24];
TH2F *hesum[24];
TH2F *hexc[24];
TH2F *hezc[24];
TH2F *hez[24];
TH2F *hxfxnc[24];
TH2F *hesumc[24];
TH2F *htx[24];
TH2F *hezg[9];
TH1F *hf[9];
TH1F *hf_all;
TH2F *hez_all;
TH2F *hezs[4];
TH2F *hxtac[24];
TH2F *hr[4];
TH2F *hrg[4];
TH2F *hrtac[4];
TH1I *hang[4];
ULong64_t add;
ULong64_t NumEntries = 0;
ULong64_t ProcessedEntries = 0;
Float_t Frac = 0.1;

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


Float_t xcoef[24] = {0.94,0.99,0.97,0.93,1.045,0.95,
		     0.91,.98,1.01, 0.95,0.99, 0,
		     0.98,1.02,1.00793,0.99,1.01,0.93,
		     0.965497,1.0, 0.92,0.985,1.05,1.08};
Float_t xstretch[24]={1,1,1,1,1,1.01,
		      0.81,0.87,0.82,0.86,1.01,1,
		      0.83,0.81,0.85,0.80,1,0.828015,
		      1,1,1,0.88,1,1.01419};
  Float_t xcenter[24]={0,0,0.01,0,0,0,
		       0.07,0.07,0.1,0,0,0,
		       0.07,0.08,0.06,0.1,0,0,
		       0,0,0,0,0,0};
Float_t ecoef[24] = {0.9,0.9, 1.05,1.09,.852,0.95,
		     1.05,0.955,0.95,0.97,0.927987,0,
		     0.9,0.92,0.922803,0.96689,1.0,.975,
		     0.896227,0.958843,1.03,0.885,1.09,0.879};

/*Float_t p0[24]={-2.869,-2.035,-1.708,-2.162,-2.047,-1.727,
		-2.693,-1.009,-2.522,-2.673,-2.052,0,
		-6.394,-6.356,-5.899,-5.544,-6.339,-6.429,
		-4.229,-4.150,-3.635,-3.978,-3.826,-3.812};
*/
Float_t p0[24]={47.474,48.28,49.179,49.570,49.314,49.955,
		47.279,48.648,48.188,47.882,48.685,0,
		46.905,47.154,48.592,49.127,48.487,48.961,
		47.398,47.419,48.553,48.474,48.334,48.490};
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

/////e calibrations///////
/////////////////////////////////////////
// Float_t ep0[24]={0.110497,-0.03,0.213181,1.2,0.640,0.4,
// 		 0.42932,0.15,0,0.63,0.853,0,
// 		 0.547691,0.45,0.3013,0.6993,1.07,0.52,
// 		 0.244061,0.2,0.201385,0.8282,0.52,0.43};
// Float_t ep1[24]={0.00344027,0.00382559,0.00404952,0.00349806,0.0035765,0.00412867,
// 		 0.00366834,0.00368933,0.00375056,0.003814,0.00373899,0,
// 		 0.00343868,0.00345896,0.00359138,0.00380036,0.00345367,0.00387393,
// 		 0.00350118,0.00360443,0.00375562,0.0034614,0.00387696,0.00388486};
// Float_t ep2[24]={-32.251,6.9,39.9,-6.9,17.6864,-171.027,
// 		 36.2922,19.542,23.503,13.4,23.4317,0,
// 		 13.3247,-34.0011,13.9187,-2.24,22.0507,-21.1745,
// 		 31.6063,-33.7536,-23.5623,18.3936,0.8,1.55357};
// Float_t ep3[24]={0,0,0,0,0,139.794,
// 		 0,0,0,0,0,0,
// 		 0,0,0,0,0,0,
// 		 0,0,0,0,0,0};
Float_t ep0[24]={0.514816,0.460846,0.596502,0.548282,0.600,0.4,
		 0.60414,0.651181,0.53368,0.289059,0.813,0,
		 0.603175,0.668597,0.509103,0.60944,1.03,0.52,
		 0.62862,0.539504,0.661149,0.409447,0.48,0.43};
Float_t ep1[24]={0.00361954,0.00391184,0.00384472,0.00419951,0.0035765,0.00412867,
		 0.00406082,0.00381772,0.00385497,0.00400755,0.00373899,0,
		 0.0039205,0.00374873,0.00397163,0.00389666,0.00345367,0.00387393,
		 0.00363259,0.0038533,0.00394617,0.0037813,0.00387696,0.00388486};
Float_t ep2[24]={-32.251,6.9,39.9,-6.9,11.789,39.6482,
		 36.2922,19.542,23.503,14.6892,23.4317,0,
		 13.3247,-34.0011,13.9187,-15.8526,21.0526,22.4894,
		 31.6063,-33.7536,-23.5623,38.3936,-11.2209,-6.93722};
Float_t ep3[24]={0,0,0,0,0,0,
		 0,0,0,0,0,0,
		 0,0,0,0,0,0,
		 0,0,0,0,0,0};
Float_t active=50.5; //Length of active area in mm
Int_t offset=-300; //Distance in mm between active detector area and target
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
    htx[i]=new TH2F(Form("htx%d",i),Form("t vs x for det %i",i),512,-2,2,512,-200,200);
    hesum[i]=new TH2F(Form("hesum%d",i),Form("e vs xf+xn for det %i",i),512,0,4000,512,0,4000);
    hxfxnc[i]=new TH2F(Form("hxfxnc%d",i),Form("xf vs xn for det %i",i),512,0,4000,512,0,4000);
    hexc[i]=new TH2F(Form("hexc%d",i),Form("e vs x for det %i",i),512,-2,2,512,0,12);
    hesumc[i]=new TH2F(Form("hesumc%d",i),Form("e vs xf+xn for det %i",i),512,0,4000,512,0,4000);
    hxtac[i]=new TH2F(Form("hxtac%d",i),Form("tac vs x gated on time difference for det %i",i),500,-0.5,1.5,512,0,4000);
    hezc[i]=new TH2F(Form("hezc%d",i),Form("e vs z corrected for det %i",i),1024,-900,0,5120,0,12);
    hez[i]=new TH2F(Form("hez%d",i),Form("e vs z for det %i",i),1024,-900,0,5120,0,3000);
  }
  for(Int_t i=0;i<9;i++){
  hezg[i]=new TH2F(Form("hezg%d",i),Form("gated e vs z for turn ID# %i",i),512,-1000,0,512,0,12);
  hf[i]=new TH1F(Form("hf%d",i),Form("excitation energy for tac peak %d",i),512,-15,15);
  }
  hez_all=new TH2F("hez","e vs z ungated",1024,-1000,0,512,0,12);
  hf_all=new TH1F("hf_all","excitation energy for all turns",512,-15,15);

  for(Int_t i=0;i<4;i++){
    hr[i]=new TH2F(Form("hr%d",i),Form("Recoil DE vs E recoil %d",i),512,0,8000,512,0,8000);
    hrg[i]=new TH2F(Form("hrg%d",i),Form("Recoil DE vs E recoil gated %d",i),512,0,8000,512,0,8000);
    hezs[i]=new TH2F(Form("hezs%d",i),Form("E vs Z for side %d",i),1024,-1000,0,1200,0,12);
    hrtac[i]=new TH2F(Form("hrtac%d",i),Form("recoil tac vs array channel for recoil id%d",i),1024,-200,200,24,0,24);
    hang[i]=new TH1I(Form("hang%d",i),Form("Angular dist for peak %d",i),180,0,180);
  }
  
  
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
  // via the fObject pointer.//
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

  Int_t eid=-1;
  Int_t xfid=-1;
  Int_t xnid=-1;
  Int_t emult=0;
  Int_t xfmult=0;
  Int_t xnmult=0;

  
  ////////////Array Diagnostic Histograms////////////////
    
    
  for(Int_t i=0;i<24;i++){
    if(e[i]>0&&(xf[i]!=0||xn[i]!=0)){
      eid=i;
      emult++;
    }
  }
  if(emult>1) faults++;
  if(eid>-1){
    //          if(event_type==14||event_type==15){
    raw.e=e[eid];
    raw.xf=xf[eid];
    raw.xn=xn[eid];
    Float_t xsum=raw.xf+raw.xn;
    Float_t xdiff=raw.xf-raw.xn;
    raw.x=0.5*(1.0+xdiff/xsum);
    //if(raw.x>1.0) cout<<"X value over 1. XF "<<raw.xf<<" XN "<<raw.xn<<endl;
    hxfxn[eid]->Fill(raw.xf,raw.xn);
    // hevx[eid]->Fill(raw.x,raw.e);
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
    Float_t excorr=ep2[eid]*pow(cal.x,2)+ep3[eid]*pow(cal.x,3);
    // cout<<excorr<<endl;
    //   corr.e-=excorr;
    corr.e*=ep1[eid];
      corr.e+=ep0[eid];
   

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
    //if((event_type==1||event_type==2||event_type==3||event_type==4)&&rmult==0) faults++;
    if(rid!=-1) hr[rid]->Fill(raw.re,raw.de);

    
    //////////////////////////////////////////////

    /////////////////Timing///////////////////////
    b_RDTTimestamp->GetEntry(entry);
    b_EnergyTimestamp->GetEntry(entry);
    b_EnergyTimestampCorrection->GetEntry(entry);
    b_RDTTimestampCorrection->GetEntry(entry);
    b_tac->GetEntry(entry);
    b_tac_t->GetEntry(entry);

    if(eid>-1&&rid>-1){
      t.e=e_t[eid];
      t.de=rdt_t[rid*2];
      t.etc=e_tc[eid];
      t.detc=rdt_tc[rid*2];
    
      Float_t time_rel=999;
      Float_t time_corr=0;
      Float_t xtcorr=p0[eid]+p1[eid]*cal.x+p2[eid]*pow(cal.x,2)+p3[eid]*pow(cal.x,3)+p4[eid]*pow(cal.x,4)+p5[eid]*pow(cal.x,5)+p6[eid]*pow(cal.x,6);
      time_rel=t.e-t.de;
      time_corr=t.etc-t.detc;
      time_rel+=time_corr;
      time_rel-=xtcorr;
      Int_t det=eid%6;
      Float_t timeshift[4][6]={{9.03223e-01,2.04866e+00,2.16988e+00,1.88652e+00,2.09173e+00,2.81914e+00},{-3.00288e-01,-3.50581e-01,2.24255e-01,-1.65042e-01,-7.79185e-02,1.02307e-01},{-2.71881e+00,-2.83466e+00,-2.29417e+00,-1.80865e+00,-2.25875e+00,-2.39437e+00},{9.66999e-01,2.84868e+00,1.79646e+00,1.27382e+00,1.83388e+00,0}};
      //time_rel-=timeshift[rid][det];
      if(raw.de>0) hrtac[rid]->Fill(time_rel,eid);
      //  time_rel*=10;
      // cout<<t.etc<<" "<<t.detc<<endl;
      htx[eid]->Fill(cal.x,time_rel);
      hez_all->Fill(cal.z,corr.e );
      Int_t idturn=-999;
      // /////////////////Position 1///////////////////
      // if(time_rel>-7.62&&time_rel<-6.05) idturn=0;
      // if(time_rel>-3.32&&time_rel<-2.15) idturn=1;
      // if(time_rel>-0.98&&time_rel<1.37) idturn=2;
      // if(time_rel>3.32&&time_rel<4.10) idturn=3;
      // if(time_rel>5.66&&time_rel<6.05) idturn=4;
      // if(time_rel>8.01&&time_rel<9.96) idturn=5;
      // if(time_rel>11.13&&time_rel<12.70) idturn=6;
      // if(time_rel>13.87&&time_rel<15.04) idturn=7;
      // ////////////////////////////////////////////

      // ////////////////Position 2//////////////////
      // if(time_rel>-4.88&&time_rel<-3.71) idturn=0;
      // if(time_rel>-2.54&&time_rel<-0.98) idturn=1;
      // if(time_rel>0.20&&time_rel<2.15) idturn=2;
      // if(time_rel>3.32&&time_rel<4.49) idturn=3;
      // if(time_rel>5.27&&time_rel<7.23) idturn=4;
      // if(time_rel>8.40&&time_rel<9.96) idturn=5;
      // if(time_rel>10.74&&time_rel<12.30) idturn=6;
      // if(time_rel>13.87&&time_rel<15.43) idturn=7;
      // ////////////////////////////////////////////

      ////////////////(d,p)//////////////////////
      if(time_rel>-1.37&&time_rel<1.37) idturn=1;
      if(time_rel>1.37&&time_rel<3.71) idturn=2;
      if(time_rel>4.88&&time_rel<6.05) idturn=3;
      if(time_rel>7.23&&time_rel<8.40) idturn=4;
      if(time_rel>9.57&&time_rel<11.52) idturn=5;
      ////////////////////////////////////////////

      //if(time_rel>-3.32&&time_rel<-2.15){//position 1
      //	if(time_rel>-2.54&&time_rel<-0.98){ //position 2
      //if(time_rel>-0.59&&time_rel<0.59){ //dp
      if(idturn!=-999) hezg[idturn]->Fill(cal.z,corr.e);
      Int_t side=floor(eid/6);
      Float_t fit0=0.365440;
      Float_t fit1=0.98488/0.965498;
      Float_t ecm=corr.e+5.28567-0.0126579*cal.z/idturn;
      Float_t ex=(11.4838+4.2190-1.0373*ecm+fit0)*fit1;
      //if(rid==0&&side==0&&raw.de>3100&&raw.re>300){//position1
      // if(rid==0&&side==0&&raw.de>3200){//position2
	  //    if(idturn==0){
      Bool_t sidecorr=0;
      Int_t ang;
      if(rid==0&&side==0&&raw.de>0) sidecorr=1;
      if(rid==1&&side==3&&raw.re>0&&raw.de>0) sidecorr=1;
      if(rid==2&&side==2&&raw.re>0&&raw.de>0) sidecorr=1;
      if(rid==3&&side==1&&raw.re>0&&raw.de>0) sidecorr=1;
	  if(sidecorr){//dp
	  hezs[side]->Fill(cal.z,corr.e);	
	  hrg[rid]->Fill(raw.re,raw.de);
	  if(idturn==1&&ex>-.15&&ex<.26){
	    ang=(det-2)*5.25+14.5;
	    hang[1]->Fill(ang);
	  }
	  if(idturn==1&&ex>0.79&&ex<1.2){
	    ang=(det-2)*4.375+15;
	    hang[2]->Fill(ang);
	  }
	  if(idturn>0){
	    hf_all->Fill(ex);
	    hf[idturn]->Fill(ex);
	    hezc[eid]->Fill(cal.z,corr.e);
	    hez[eid]->Fill(cal.z,cal.e);
	    hevx[eid]->Fill(cal.x,cal.e);
	    hexc[eid]->Fill(cal.x,corr.e);
	  }
	  if(tac_t[0]-t.e>640&&tac_t[0]-t.e<693){
	    hxtac[eid]->Fill(cal.x,tac[0]);
	  }
	}
	//	if(rid==1&&side==3&&raw.de>1200&&raw.re>2000){//position1
	// if(rid==1&&side==3&&raw.de>1200){//position2

	//	if(rid==2&&side==2&&raw.de>3100&&raw.re>800){//position1
	//if(rid==2&&side==2&&raw.de>2800){//position2

	//	if(rid==3&&side==1&&raw.de>1400&&raw.re>2400){//position1
	//  if(rid==3&&side==1&&raw.de>1300){//position2

	//	  }
    }
   
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
  cout<<processed<<" Events Processed. "<<faults<<" sort3 errors."<<endl;
}
