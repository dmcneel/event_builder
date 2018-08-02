{
  TFile *file1=new TFile("z1/position1.root");
  TFile *file2=new TFile("z2/position2.root");
  file1->cd();
  TH2F *hez1=(TH2F*) file1->Get("hezg2");
  TH1F *hf1=(TH1F*) file1->Get("hf");
  TCutG *cut1=(TCutG*) file1->Get("naccept1");

  file2->cd();
  TH2F *hez2=(TH2F*) file2->Get("hezg0");
  TH1F *hf2=(TH1F*) file2->Get("hf");
  TCutG *cut2=(TCutG*) file2->Get("naccept2");
  new TCanvas("c1","c1");
  new TCanvas("c2","c2");
  new TCanvas("c3","c3");
  new TCanvas("c4","c4");
  new TCanvas("c5","c5");
  c1->cd();
  hez1->SetMarkerStyle(kFullCircle);
  hez1->SetMarkerColor(kBlack);
  hez1->SetStats(0);
  hez2->SetStats(0);
  hez2->SetMarkerStyle(kFullCircle);
  hez2->SetMarkerColor(kBlack);
  hez2->Draw();
  hez1->Draw("same");
  c2->cd();
  hf1->SetLineColor(2);
  hf1->SetFillColor(2);
  hf1->SetFillStyle(3001);
    hf2->SetLineColor(4);
  hf2->SetFillColor(4);
  hf2->SetFillStyle(3001);
  TH1F h3=*hf1+*hf2;
  h3->SetFillColor(1);
  h3->SetLineColor(1);
  h3->SetFillStyle(3001);
  h3->Draw();
  hf2->Draw("same");
  hf1->Draw("same");
  c3->cd();
  hez1->SetMarkerColor(2);
  hez1->Draw();
  c4->cd();
  hez2->SetMarkerColor(4);
  hez2->Draw();
  c5->cd();
  c5->Divide(1,2);
  c5->cd(1);
  hf1->SetAxisRange(-1,15,"X");
  hf2->SetAxisRange(-1,15,"X");
  hf1->Draw();
  c5->cd(2);
  hf2->Draw();
}
