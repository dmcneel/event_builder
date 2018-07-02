
{
  new TCanvas("c1","c1");
 	gStyle->SetOptStat(0);	

  Int_t rangelow=0;
  Int_t rangehigh=0;

  TF1 *f1=new TF1("f1","pol3",-1,-2,"");

 
 
  Float_t exx1[24];
  Float_t exx2[24];
  Float_t exx3[24];
  
  for(Int_t detid=0;detid<24;detid++){
    exx1[detid]=0;
    exx2[detid]=0;
    exx3[detid]=0;
    hxec[detid]->SetAxisRange(0,1,"X");
    hxec[detid]->SetAxisRange(0,3000,"Y");
 

    hxec[detid]->SetMarkerStyle(kFullCircle);
    hxec[detid]->Draw();
    c1->Update();
    c1->WaitPrimitive("CUTG","CutG");
    CUTG->SetName("fit");

  
    hxec[detid]->ProfileX("xprof1",0,512,"[fit]");


    f1->SetRange(0,1);
    xprof1->Fit("f1","R");
    xprof1->Draw();
    c1->Update();
    c1->WaitPrimitive();
    
   
    exx1[detid]=(Float_t)f1->GetParameter(1);
    exx2[detid]=(Float_t)f1->GetParameter(2);
    exx3[detid]=(Float_t)f1->GetParameter(3);
	   
    cout<<"calibration for detid "<<detid<<" ez1 = "<<ex1[detid]<< " ez2= "<<ex2[detid]<<" ez3= "<<ex3[detid]<<endl;
    hxec[detid]->Draw();
    f1->Draw("same");

    c1->Update();
    c1->WaitPrimitive(); 
    fit->Delete();
    c1->Clear();

  }
}
