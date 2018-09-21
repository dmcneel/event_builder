void binshift(Char_t *histname1,Char_t * histname2, Float_t shiftval)
{
  TH1 *hist1=(TH1 *) gROOT->FindObject(histname1);
  TH1 *hist2=(TH1 *) gROOT->FindObject(histname2);
  Int_t binshift=shiftval/hist1->GetBinWidth(1);
  cout<<"Shift in bins is "<<binshift<<endl;
  for (Int_t i=0; i< hist1->GetNbinsX();i++) 
    {    
      Float_t binvalue=hist1->GetBinContent(i);
      if ((i-binshift)>0 && i+binshift<hist1->GetNbinsX()){
	hist2->SetBinContent(i+binshift,binvalue);}
      else hist2->SetBinContent(i+binshift,0);
    }
}

void chisq(Char_t *histname1, Char_t *histname2,Float_t lowx=-999, 
	   Float_t hix=-999)
{
  TH1 *hist1=(TH1 *) gROOT->FindObject(histname1);
  TH1 *hist2=(TH1 *) gROOT->FindObject(histname2);
  Float_t chisquare;
  Float_t minbin,maxbin;
  if (lowx==-999) 
    {
      minbin=0;
    } else 
    {
      minbin=hist1->FindBin(lowx);
    }
  if (hix==-999) 
    {
      minbin=0;
    } else 
    {
      maxbin=hist1->FindBin(hix);
    }
  for (Int_t i=minbin;i<maxbin;i++) {
    Float_t binval1=hist1->GetBinContent(i);
    Float_t binval2=hist2->GetBinContent(i);
    if (binval1>0) {
      chisquare=chisquare+(binval1-binval2)*(binval1-binval2)/binval1;
    }
  }
  hist1->Draw("err");
  hist2->SetLineColor(2);
  hist2->Draw("same");
    cout<<"Total chi square= "<<chisquare<<endl;
    cout<<"Reduced chi square="<<chisquare/(maxbin-minbin-1);
}
