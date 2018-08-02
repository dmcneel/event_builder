
{
  new TCanvas("c1","c1");
 

  Int_t rangelow=0;
  Int_t rangehigh=0;
  Double_t mid=0;
  Double_t m=0;
  Double_t b=0;
  TF1 *f1=new TF1("f1","pol1",-900,0);
  TF1 *f2=new TF1("f2","pol1",-900,0);
  TF1 *f3=new TF1("f3","pol1",-900,0);
  Float_t hslope=0.0126579;
  Float_t eslope=0.013053;
  f3->SetParameters(10.4,eslope);
  Int_t sldiff=eslope-hslope;
  Float_t e0[24];
  Float_t e1[24];
  
  for(Int_t i=0;i<4;i++){
    // if(detid%6<2) break;
    Int_t detid=i*6+3;
    rangelow=-450+59*(detid%6);
    rangehigh=-390+59*(detid%6);
    hez[detid]->SetAxisRange(rangelow,rangehigh,"X");
    hez[detid]->SetAxisRange(0,3000,"Y");
    hez[detid]->SetMarkerStyle(kFullCircle);
    hez[detid]->Draw();
    c1->Update();
    c1->WaitPrimitive("CUTG","CutG");
    CUTG->SetName("gs");

    c1->WaitPrimitive("CUTG","CutG");
    
    CUTG->SetName("ex1");
    hez[detid]->ProfileX("xprof1",0,5120,"[gs]");
    hez[detid]->ProfileX("xprof2",0,5120,"[ex1]");
    f1->SetRange(rangelow,rangehigh);
    f2->SetRange(rangelow,rangehigh);
    xprof1->Fit("f1","QRME");
    xprof1->Draw();
    c1->Update();
    c1->WaitPrimitive();
    f2->FixParameter(1,f1->GetParameter(1));
    xprof2->Fit("f2","QRBME");
    xprof2->Draw();
    c1->Update();
    c1->WaitPrimitive();
    mid=(rangelow+rangehigh)/2;
    m=0.98448/(f1->Eval(mid)-f2->Eval(mid));
    b=(f3->Eval(mid)+TMath::Abs(mid*sldiff))-m*f1->Eval(mid);
    e0[detid]=b;
    e1[detid]=m;
    cout<<"calibration for detid "<<detid<<" slope = "<<m<< " intercept= "<<b<<endl;
    hez[detid]->Draw();
    f1->Draw("same");
    f2->Draw("same");
    c1->Update();
    c1->WaitPrimitive();
    gs->Delete();
    ex1->Delete();
    c1->Clear();

  }
}
