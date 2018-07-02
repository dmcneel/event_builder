
{
  new TCanvas("c1","c1");
 

  Int_t rangelow=0;
  Int_t rangehigh=0;
  Double_t mid=0;
  Double_t m=0;
  Double_t b=0;
  TF1 *f1=new TF1("f1","pol1",-850,-500,"");
  TF1 *f2=new TF1("f2","pol1",-850,-500,"");
  TF1 *f3=new TF1("f3","pol1",-850,0,"");
  f3->SetParameters(8.3481,0.0051805);
  Float_t e0[24];
  Float_t e1[24];
  
  for(Int_t detid=0;detid<24;detid++){
  
    rangelow=-850+59*(detid%6);
    rangehigh=-790+59*(detid%6);
    hze[detid]->SetAxisRange(rangelow,rangehigh,"X");
    hze[detid]->SetAxisRange(0,3000,"Y");
    hze[detid]->SetMarkerStyle(kFullCircle);
    hze[detid]->Draw();
    c1->Update();
    c1->WaitPrimitive("CUTG","CutG");
    CUTG->SetName("gs");

    c1->WaitPrimitive("CUTG","CutG");
    
    CUTG->SetName("ex1");
    hze[detid]->ProfileX("xprof1",0,512,"[gs]");
    hze[detid]->ProfileX("xprof2",0,512,"[ex1]");
    f1->SetRange(rangelow,rangehigh);
    f2->SetRange(rangelow,rangehigh);
    xprof1->Fit("f1","RME");
    xprof1->Draw();
    c1->Update();
    c1->WaitPrimitive();
    f2->FixParameter(1,f1->GetParameter(1));
    xprof2->Fit("f2","RBME");
    xprof2->Draw();
    c1->Update();
    c1->WaitPrimitive();
    mid=(rangelow+rangehigh)/2;
    m=0.98448/(f1->Eval(mid)-f2->Eval(mid));
    b=f3->Eval(mid)-m*f1->Eval(mid);
    e0[detid]=b;
    e1[detid]=m;
    cout<<"calibration for detid "<<detid<<" slope = "<<m<< " intercept= "<<b<<endl;
    hze[detid]->Draw();
    f1->Draw("same");
    f2->Draw("same");
    c1->Update();
    c1->WaitPrimitive();
    gs->Delete();
    ex1->Delete();
    c1->Clear();

  }
}
