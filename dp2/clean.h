//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun  4 16:42:39 2018 by ROOT version 6.12/06
// from TTree gen_tree/PSD Tree
// found on file: gen_run400.root
//////////////////////////////////////////////////////////

#ifndef clean_h
#define clean_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TCanvas.h>
#include <TCutG.h>
#include <TH1.h>
#include <TH2.h>
#include <TStopwatch.h>
// Header file for the classes stored in the TTree if any.

class clean : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           event_type;
   Float_t         e[100];
   ULong64_t       e_t[100];
   Float_t         e_tc[100];
   Float_t         a_rise[100];
   Float_t         xf[100];
   ULong64_t       xf_t[100];
   Float_t         xn[100];
   ULong64_t       xn_t[100];
   Float_t         rdt[100];
   ULong64_t       rdt_t[100];
   Float_t         rdt_tc[100];
   Float_t         rdt_rise[100];
   Float_t         tac[100];
   ULong64_t       tac_t[100];
   Float_t         elum[32];
   ULong64_t       elum_t[32];
   Float_t         ezero[32];
   ULong64_t       ezero_t[32];

   // List of branches
     TBranch        *b_event_type;   //!
   TBranch        *b_Energy;   //!
   TBranch        *b_EnergyTimestamp;   //!
   TBranch        *b_EnergyTimestampCorrection;   //!
    TBranch        *b_ArrayRise;   //!
   TBranch        *b_XF;   //!
   TBranch        *b_XFTimestamp;   //!
   TBranch        *b_XN;   //!
   TBranch        *b_XNTimestamp;   //!
   TBranch        *b_RDT;   //!
   TBranch        *b_RDTTimestamp;   //!
   TBranch        *b_RDTTimestampCorrection;   //!
     TBranch        *b_RDTRise;   //!
   TBranch        *b_tac;   //!
   TBranch        *b_tac_t;   //!
   TBranch        *b_ELUM;   //!
   TBranch        *b_ELUMTimestamp;   //!
   TBranch        *b_EZERO;   //!
   TBranch        *b_EZEROTimestamp;   //!

   clean(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~clean() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(clean,0);
};

#endif

#ifdef clean_cxx
void clean::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

     fChain->SetBranchAddress("event_type", &event_type, &b_event_type);
   fChain->SetBranchAddress("e", e, &b_Energy);
   fChain->SetBranchAddress("e_t", e_t, &b_EnergyTimestamp);
   fChain->SetBranchAddress("e_tc", e_tc, &b_EnergyTimestampCorrection);
   fChain->SetBranchAddress("a_rise", a_rise, &b_ArrayRise);
   fChain->SetBranchAddress("xf", xf, &b_XF);
   fChain->SetBranchAddress("xf_t", xf_t, &b_XFTimestamp);
   fChain->SetBranchAddress("xn", xn, &b_XN);
   fChain->SetBranchAddress("xn_t", xn_t, &b_XNTimestamp);
   fChain->SetBranchAddress("rdt", rdt, &b_RDT);
   fChain->SetBranchAddress("rdt_t", rdt_t, &b_RDTTimestamp);
   fChain->SetBranchAddress("rdt_tc", rdt_tc, &b_RDTTimestampCorrection);
     fChain->SetBranchAddress("rdt_rise", rdt_rise, &b_RDTRise);
   fChain->SetBranchAddress("tac", &tac, &b_tac);
   fChain->SetBranchAddress("tac_t", &tac_t, &b_tac_t);
   fChain->SetBranchAddress("elum", elum, &b_ELUM);
   fChain->SetBranchAddress("elum_t", elum_t, &b_ELUMTimestamp);
   fChain->SetBranchAddress("ezero", ezero, &b_EZERO);
   fChain->SetBranchAddress("ezero_t", ezero_t, &b_EZEROTimestamp);
}

Bool_t clean::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef clean_cxx
