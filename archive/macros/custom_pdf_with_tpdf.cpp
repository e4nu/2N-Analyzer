void custom_pdf_with_tpdf() {
    TCanvas* canvas = new TCanvas("canvas");
    TH1F* histo = new TH1F("histo","test 1",10,0.,10.);
    histo->SetFillColor(2);
    histo->Fill(2.);
    histo->Draw();
    canvas->Print("plots.pdf(","Title:One bin filled");
    histo->Fill(4.);
    histo->Draw();
    canvas->Print("plots.pdf","Title:Two bins filled");
    histo->Fill(6.);
    histo->Draw();
    canvas->Print("plots.pdf","Title:Three bins filled");
    histo->Fill(8.);
    histo->Draw();
    canvas->Print("plots.pdf","Title:Four bins filled");
    histo->Fill(8.);
    histo->Draw();
    canvas->Print("plots.pdf)","Title:The fourth bin content is 2");

//    // Create a TPDF object for full control over PDF output
////    TPDF *pdf = new TPDF();
//    TPDF *pdf = new TPDF("output.pdf");
////    TPDF *pdf = new TPDF("output.pdf", 112);
////    TPDF *pdf = new TPDF("output.pdf", 111);
//
//    // Create histograms
//    TH1F *h1 = new TH1F("h1", "Gaussian Distribution;X-axis;Y-axis", 50, -4, 4);
//    TH1F *h2 = new TH1F("h2", "Exponential Distribution;X-axis;Y-axis", 50, 0, 5);
//
//    // Fill histograms
//    h1->FillRandom("gaus", 1000);
//    h2->FillRandom("expo", 1000);
//
//    // Create a canvas
//    TCanvas *c = new TCanvas("c", "Canvas", 800, 600);
//
//    pdf->SetTitle("Custom Title Page");  // Set the title for the first page
//    
//    // Page 1: Gaussian Distribution
////    pdf->NewPage();  // Start a new page
//    pdf->SetTitle("Gaussian Distribution");  // Explicitly set the bookmark title
//    h1->Draw();
////    c->Clear();
//    c->Update();
//
//    // Page 2: Exponential Distribution
////    pdf->NewPage();  // Start another new page
//    pdf->SetTitle("Exponential Distribution");  // Explicitly set the second bookmark title
//    h2->Draw();
////    c->Clear();
//    c->Update();
//
//    // Close the PDF file
//    delete pdf;
//
//    // Clean up
//    delete h1;
//    delete h2;
//    delete c;
//
//    std::cout << "PDF generated with correct bookmarks using TPDF." << std::endl;
//    
//    // Create a TPDF object for full control over PDF output
//    TPDF *pdf = new TPDF("output.pdf", 111);
//
//    // Create histograms
//    TH1F *h1 = new TH1F("h1", "Gaussian Distribution;X-axis;Y-axis", 50, -4, 4);
//    TH1F *h2 = new TH1F("h2", "Exponential Distribution;X-axis;Y-axis", 50, 0, 5);
//
//    // Fill histograms
//    h1->FillRandom("gaus", 1000);
//    h2->FillRandom("expo", 1000);
//
//    // Create a canvas
//    TCanvas *c = new TCanvas("c", "Canvas", 800, 600);
//
//    // Page 1: Gaussian Distribution
//    pdf->NewPage();  // Start a new page
//    pdf->SetTitle("Gaussian Distribution");  // Explicitly set the bookmark title
//    h1->Draw();
//    c->Update();
//
//    // Page 2: Exponential Distribution
//    pdf->NewPage();  // Start another new page
//    pdf->SetTitle("Exponential Distribution");  // Explicitly set the second bookmark title
//    h2->Draw();
//    c->Update();
//
//    // Close the PDF file
//    delete pdf;
//
//    // Clean up
//    delete h1;
//    delete h2;
//    delete c;
//
//    std::cout << "PDF generated with correct bookmarks using TPDF." << std::endl;
}
