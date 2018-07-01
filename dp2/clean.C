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

TH2F *hxe[24];
TH2F *hze[24];
TH2F *hzec[24];
TH2F *hxfxn[24];
TH2F *hesum[24];
TH2F *hxec[24];
TH2F *hxect;
TH2F *hxfxnc[24];
TH2F *hesumc[24];
TH2F *htx[24];
TH2F *hezg[9];
TH3F *hetz[4];
TH2F *hez;
TH2F *hezs[4];
TH2F *hxtac[24];
TH2F *hr[4];
TH2F *hrg[4];
TH1I *hrtac;
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

// Float_t xcoef[24] = {0.949029,1.01802,0.971783,0.927059,0.991241,0.881107,
// 		     0.906056,0.926736,0.989037, 1.00974,0.950697, 0,
// 		     1.03673,1.00772,1.00793,0.970457,0.955211,1.12365,
// 		     0.965497,0.977822, 0.889425,0.983675,1.08844,1.09201};
Float_t xcoef[24] = {0.94,0.99,0.97,0.93,1.045,0.95,
		     0.91,.98,1.01, 0.95,0.99, 0,
		     0.98,1.00772,1.00793,0.99,1.01,0.93,
		     0.965497,1.0, 0.92,0.985,1.05,1.08};
Float_t ecoef[24] = {0.9,0.9, 1.05,1.09,.852,0.95,
		     1.05,0.955,0.95,0.97,0.927987,0,
		     0.9,0.927974,0.922803,0.96689,1.0,.975,
		     0.896227,0.958843,1.03,0.885,1.09,0.879};

///xt calibrations////
Float_t p0[24]={-2.869,-2.035,-1.708,-2.162,-2.047,-1.277,
		-2.215,-1.118,-2.631,-2.563,-1.674,0,
		-5.831,-6.356,-6.727,-6.171,-6.339,-6.429,
		-4.229,-4.150,-3.635,-3.978,-3.826,-3.812};
Float_t p1[24]={3.9303,-2.84079,-0.461684,17.9265,28.4461,22.7594,
		-6.09744,-0.422493,5.7568,21.9062,-0.134449,0,
		-8.00939,-5.25291,-0.815182,3.6144,-1.00911,-0.832172,
		2.16823,1.65914,-0.83749,2.40862,1.55533,0.424124};
Float_t p2[24]={40.5575,99.8466,81.446,-168.315,-256.496,-201.758,
		45.5039,41.0736,7.03306,-167.383,91.019,0,
		48.4203,46.0097,27.609,14.8123,72.9097,70.865,
		40.3466,47.9315,65.2038,35.212,48.1242,45.2454};
Float_t p3[24]={-89.3624,-189.934,-160.03,1095.22,1375.2,1176.66,
		-76.6702,-77.2918,-24.1207,950.941,-180.645,0,
		-76.8551,-80.8722,-52.7005,-35.3499,-143.71,-139.787,
		-86.1288,-100.492,-126.583,-74.8577,-100.214,-90.8535};
Float_t p4[24]={45.0386,92.7441,79.052,-2528.57,-2930.57,-2555.62,
		36.9702,36.395,11.1158,-2106.76,89.7875,0,
		35.5022,40.0052,26.113,16.4546,71.6839,69.8255,
		43.6401,50.9485,62.2398,37.2459,50.496,45.0125};
Float_t p5[24]={0,0,0,2376.91,2638.12,2289.62,
		0,0,0,1936.95,0,0,
		0,0,0,0,0,0,
		0,0,0,0,0,0};
Float_t p6[24]={0,0,0,-792.989,-854.609,-731.779,
		0,0,0,-635.361,0,0,
		0,0,0,0,0,0,
		0,0,0,0,0,0};
///////////////////////////

/////e calibrations///////

Float_t kcoef[24] = {0.00377855,
		     0.004016195,
		     0.003470516,
		     0.003675252,
		     0.004003605,
		     0.00408688,
		     0.003707257,
		     0.003818087,
		     0.003829535,
		     0.003745308,
		     0.004016195,
		     0.0,
		     0.004217104,
		     0.003911639,
		     0.003991094,
		     0.003806706,
		     0.003739824,
		     0.003876024,
		     0.003889303,
		     0.003737089,
		     0.003669971,
		     0.003917638,
		     0.003330248,
		     0.004080351};
Float_t bcoef[24] = {0.096813683,
		     0.065432704,
		     0.075887908,
		     0.061711223,
		     0.067195298,
		     0.07188432,
		     0.049367925,
		     0.051168909,
		     0.04944018,
		     0.054667889,
		     0.069448899,
		     0.0,
		     0.11279158,
		     0.04486585,
		     0.072937969,
		     0.075727571,
		     -0.004330307,
		     -0.06998437,
		     0.069584964,
		     0.059662546,
		     0.029494684,
		     0.047889571,
		     0.038246154,
		     0.048290096};



/////////////////////////////////////////
Float_t ep2[24]={-227.625,98.2865,0,0,0,0,
		 0,0,0,0,0,0,
		 0,0,0,0,0,0,
		 0,0,0,0,0,0};
Float_t ep3[24]={110.912,-53.7275,0,0,0,0,
		 0,0,0,0,0,0,
		 0,0,0,0,0,0,
		 0,0,0,0,0,0};


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
    hxe[i]=new TH2F(Form("hex%d",i),Form("e vs x for det %i",i),512,-2,2,512,0,4000);
    hze[i]=new TH2F(Form("hez%d",i),Form("e vs z for det %i",i),1024,-900,-400,512,0,4000);
    hzec[i]=new TH2F(Form("hezc%d",i),Form("e vs z corrected for det %i",i),1024,-900,-400,512,0,12);
    htx[i]=new TH2F(Form("htx%d",i),Form("t vs x for det %i",i),512,-2,2,512,-50,50);
    hesum[i]=new TH2F(Form("hesum%d",i),Form("e vs xf+xn for det %i",i),512,0,4000,512,0,4000);
    hxfxnc[i]=new TH2F(Form("hxfxnc%d",i),Form("xf vs xn for det %i",i),512,0,4000,512,0,4000);
    hxec[i]=new TH2F(Form("hexc%d",i),Form("e vs x for det %i",i),512,-2,2,512,0,4000);
    hesumc[i]=new TH2F(Form("hesumc%d",i),Form("e vs xf+xn for det %i",i),512,0,4000,512,0,4000);
    hxtac[i]=new TH2F(Form("hxtac%d",i),Form("tac vs x gated on time difference for det %i",i),500,-0.5,1.5,512,0,4000);
     
  }
  for(Int_t i=0;i<9;i++){
  hezg[i]=new TH2F(Form("hezg%d",i),Form("gated e vs z for turn ID# %i",i),512,-1000,0,512,0,12);
  }
  hez=new TH2F("hez","e vs z ungated",1024,-1000,0,512,0,4000);
  hrtac=new TH1I("hrtac","gated and added recoil tac",1024,-200,200);
  hxect=new TH2F("hxect"," test of energy calibration",1024,-0.1,1.1,10024,-1,1400);
  for(Int_t i=0;i<4;i++){
    hr[i]=new TH2F(Form("hr%d",i),Form("Recoil DE vs E recoil %d",i),512,0,8000,512,0,8000);
    hrg[i]=new TH2F(Form("hrg%d",i),Form("Recoil DE vs E recoil gated %d",i),512,0,8000,512,0,8000);
    hezs[i]=new TH2F(Form("hezs%d",i),Form("E vs Z for side %d",i),1024,-1000,0,512,0,12);
    hetz[i]=new TH3F(Form("het%d",i),Form("E vs time_rel vs z for side %d",i),200,-10,10,20,2,4,150,-540,-490);

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
    hxe[eid]->Fill(raw.x,raw.e);
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
  
    hxfxnc[eid]->Fill(cal.xf,cal.xn);
   
    hesumc[eid]->Fill(xsum,cal.e);
    cal.z=-positions[6-eid%6]-active/2.+positions[0]+(active*cal.x);
    corr.e=cal.e*kcoef[eid]+bcoef[eid];
    //corr.e+=ep0[eid];
   
    // cout<<cal.z<<endl;
 
  
    //   }

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
    Int_t idturn=-999;
    Float_t time_rel=999;
  
      t.e=e_t[eid];
      t.de=rdt_t[rid*2];
      t.etc=e_tc[eid];
      t.detc=rdt_tc[rid*2];
    
     
      Float_t time_corr=0;
      Float_t xtcorr=p0[eid]+p1[eid]*cal.x+p2[eid]*pow(cal.x,2)+p3[eid]*pow(cal.x,3)+p4[eid]*pow(cal.x,4)+p5[eid]*pow(cal.x,5)+p6[eid]*pow(cal.x,6);
      time_rel=t.e-t.de;
      time_corr=t.etc-t.detc;
      time_rel+=time_corr;
      time_rel-=xtcorr;
      //  time_rel*=10;
      // cout<<t.etc<<" "<<t.detc<<endl;
      if(eid>-1&&rid>-1){
	htx[eid]->Fill(cal.x,time_rel);
	hez->Fill(cal.z,cal.e);

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
	if(time_rel>-0.59&&time_rel<0.59) idturn=0;
	if(time_rel>3.32&&time_rel<4.49) idturn=1;
	if(time_rel>5.56&&time_rel<7.23) idturn=2;
	if(time_rel>8.40&&time_rel<9.18) idturn=3;
	if(time_rel>11.13&&time_rel<12.30) idturn=4;
	if(time_rel>13.48&&time_rel<14.65) idturn=5;
	if(time_rel>16.99&&time_rel<17.77) idturn=6;
	if(time_rel>18.55&&time_rel<19.73) idturn=7;
	if(time_rel>24.02&&time_rel<24.80) idturn=8;
	////////////////////////////////////////////

	//if(time_rel>-3.32&&time_rel<-2.15){//position 1
	//	if(time_rel>-2.54&&time_rel<-0.98){ //position 2
	//if(time_rel>-0.59&&time_rel<0.59){ //dp
	if(idturn!=-999) hezg[idturn]->Fill(cal.z,corr.e);
	Int_t side=floor(eid/6);
	hezs[side]->Fill(cal.z,corr.e);
	hrtac->Fill(time_rel);
	if(corr.e>2.6&&corr.e<3.3&&cal.z>-520) hetz[side]->Fill(time_rel,corr.e,cal.z);
	//if(rid==0&&side==0&&raw.de>3100&&raw.re>300){//position1
	// if(rid==0&&side==0&&raw.de>3200){//position2
   
	if(rid==0&&side==0&&idturn==0){//dp
	  //	  hezs[side]->Fill(cal.z,corr.e);
	  hrg[rid]->Fill(raw.re,raw.de);
	
	  hxec[eid]->Fill(cal.x,cal.e);
	  hzec[eid]->Fill(cal.z,corr.e);
	  //  hze[eid]->Fill(cal.z,cal.e);
	  if(eid==0) hxect->Fill(cal.x,corr.e);
	  if(tac_t[0]-t.e>640&&tac_t[0]-t.e<693){
	    hxtac[eid]->Fill(cal.x,tac[0]);
	  }
	}
	//	if(rid==1&&side==3&&raw.de>1200&&raw.re>2000){//position1
	// if(rid==1&&side==3&&raw.de>1200){//position2
	if(rid==1&&side==3&&idturn==0){//dp
	  //	  hezs[side]->Fill(cal.z,corr.e);
	  hrg[rid]->Fill(raw.re,raw.de);
	  hze[eid]->Fill(cal.z,cal.e);
	  hxec[eid]->Fill(cal.x,cal.e);
	  hzec[eid]->Fill(cal.z,corr.e);
	  if(tac_t[0]-t.e>640&&tac_t[0]-t.e<693){
	    hxtac[eid]->Fill(cal.x,tac[0]);
	  }
	}
	//	if(rid==2&&side==2&&raw.de>3100&&raw.re>800){//position1
	//if(rid==2&&side==2&&raw.de>2800){//position2
	if(rid==2&&side==2&&idturn==0){//dp
	  //	  hezs[side]->Fill(cal.z,corr.e);
	  hrg[rid]->Fill(raw.re,raw.de);
	  hze[eid]->Fill(cal.z,cal.e);
	  hxec[eid]->Fill(cal.x,cal.e);
	  hzec[eid]->Fill(cal.z,corr.e);
	  if(tac_t[0]-t.e>640&&tac_t[0]-t.e<693){
	    hxtac[eid]->Fill(cal.x,tac[0]);
	  }
	}	
	//	if(rid==3&&side==1&&raw.de>1400&&raw.re>2400){//position1
	//  if(rid==3&&side==1&&raw.de>1300){//position2
	if(rid==3&&side==1&&idturn==0){//
	  //	  hezs[side]->Fill(cal.z,corr.e);
	  hrg[rid]->Fill(raw.re,raw.de);
	  hze[eid]->Fill(cal.z,cal.e);
	  hxec[eid]->Fill(cal.x,cal.e);
	  hzec[eid]->Fill(cal.z,corr.e);
	  if(tac_t[0]-t.e>640&&tac_t[0]-t.e<693){
	    hxtac[eid]->Fill(cal.x,tac[0]);
	  }
	}
     
      }
      hze[eid]->Fill(cal.z,corr.e);
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
  cout<<processed<<" Events Processed. "<<faults<<" sort3 errors."<<endl;
}
