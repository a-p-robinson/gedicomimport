/*####################################################
  # projMacros.C                                     #
  # Version: 0.1                                     #
  #                                                  #
  # Functions to read and write projection data from #
  # ROOT files                                       #
  #                                                  #
  # APR: 09/11/11                                    #
  # andrew.paul.robinson@manchester.ac.uk            #
  ####################################################*/

// Analyse the projections (look for overflows etc)
void analyseProj(char projName[100], char type[100]){

    char hname[300];
    TH2 *hist;

    short val;

    // Loop through Head0
    cout << "Analysing " << projName << "/" << type << "/Head0/proj-* on Head0" << endl; 
 
    // Loop through Head0
    for (int i = 0; i < 30; i++){
        sprintf(hname,"%s/%s/Head0/proj-%i",projName,type,i);
        gDirectory->GetObject(hname,hist);
    
        if(hist == NULL){
            cout << "ERROR: Can not get histogram - " << hname << endl;
        }
        else{

            cout << "Checking: " << hname << endl;

            // Check for overflows and...

            // Get Nbins in each direction
            int nbinx = hist->GetXaxis()->GetNbins();
            int nbiny = hist->GetYaxis()->GetNbins();

            // Loop through each pixel
            for(int pi = 0; pi < nbinx; pi++){
                for(int pj = 0; pj < nbiny; pj++){
                    val = hist->GetBinContent(pi,pj);
                    if(val > 32766){
                        cout << "Overflow [" << hname << "] " << pi << ":" << pj << " " << val << endl;
                    }
                }
            }

        } // Close else loop
    } // Close loop through head0
  
    // Loop through Head1  
    cout << "Analysing " << projName << "/" << type << "/Head1/proj-* on Head1" << endl; 
 
    // Loop through Head1
    for (int i = 0; i < 30; i++){
        sprintf(hname,"%s/%s/Head1/proj-%i",projName,type,i);
        gDirectory->GetObject(hname,hist);
    
        if(hist == NULL){
            cout << "ERROR: Can not get histogram - " << hname << endl;
        }
        else{

            cout << "Checking: " << hname << endl;

            // Check for overflows and...

            // Get Nbins in each direction
            int nbinx = hist->GetXaxis()->GetNbins();
            int nbiny = hist->GetYaxis()->GetNbins();

            // Loop through each pixel
            for(int pi = 0; pi < nbinx; pi++){
                for(int pj = 0; pj < nbiny; pj++){
                    val = hist->GetBinContent(pi,pj);
                    if(val > 32766){
                        cout << "Overflow [" << hname << "] " << pi << ":" << pj << " " << val << endl;
                    }
                }
            }

        } // Close else loop
    } // Close loop through head1

    // for (int i = 0; i < 30; i++){
    //    sprintf(hname,"%s/%s/Head1/proj-%i",projName,type,i);
    //    gDirectory->GetObject(hname,hist);
    
    //    if(hist == NULL){
    //      cout << "ERROR: Can not get histogram - " << hname << endl;
    //    }
    //    else{

    //      // Check for overflows and...

    //    }
    //  }

}


// Loop through a projection and fill an time histogram
void getTime(char projName[100], char type[100], char outputHist[100], int maxTime){

    char hname[300];
    TH2 *hist;

    // Make a new histogram
    TH1D *ttime = new TH1D(outputHist,outputHist,30,0,30);

    // Loop through Head0
    cout << "Looping through " << projName << "/" << type << "/Head0/proj-* on Head0" << endl; 
    cout << "Looping through " << projName << "/" << type << "/Head1/proj-* on Head1" << endl; 

    for (int i = 0; i < 30; i++){

        float sumcounts = 0;

        sprintf(hname,"%s/%s/Head0/proj-%i",projName,type,i);
        gDirectory->GetObject(hname,hist);
      
        if(hist == NULL){
            cout << "ERROR: Can not get histogram - " << hname << endl;
        }
        else{
            cout << "Head0: " << i << " : " << hist->GetSumOfWeights() << endl;
            sumcounts += hist->GetSumOfWeights();
        }

        sprintf(hname,"%s/%s/Head1/proj-%i",projName,type,i);
        gDirectory->GetObject(hname,hist);
      
        if(hist == NULL){
            cout << "ERROR: Can not get histogram - " << hname << endl;
        }
        else{
            cout << "Head1: " << i << " : " << hist->GetSumOfWeights() << endl;
            sumcounts += hist->GetSumOfWeights();
            ttime->Fill(i, sumcounts);
        }

    }
 
    // Set the range
    ttime->GetXaxis()->SetLimits(0,maxTime);

}



// Show Scatter Fraction
void showSF(char projName[100], char type[100], int pal = 0, int bg = 0, int proj = -99, int source = 0){

    char hname[300];
    TH2 *hist;

    TPaletteAxis *colpal;

    //--- Define a user defined palette ---
    // 100 step from Black(0) to white (1)
    TColor *color;
    Int_t palette[100];
    int i,j;
    double Wcol;
  
    // color 51-299 are available ?
    for (int i=0;i<100;i++) {
        Wcol = Float_t(i)/99;
        color = new TColor(1000+i,Wcol,Wcol,Wcol,"");
        palette[i] = 1000+i;
    }
    //-------------------------------------

    //--- Select Pallette - deafult is black and white ---
    if (pal == 0){
        gStyle->SetPalette(100,palette);
    }
    if (pal == 1){
        gStyle->SetPalette(1);
    }
    //-------------------------------------
  
    //-- Set up canvases ------------------
    Double_t w = 800;
    Double_t h = 960;
    TCanvas * c1 = new TCanvas("head0", "head0", w, h);
    c1->SetWindowSize(w + (w - c1->GetWw()), h + (h - c1->GetWh()));

    TCanvas * c2 = new TCanvas("head1", "head1", w, h);
    c2->SetWindowSize(w + (w - c2->GetWw()), h + (h - c2->GetWh()));
    //-------------------------------------

    // Show all 60 projections
    if (proj < 0){

        // Divide canvases
        c1->Divide(5,6);
        c2->Divide(5,6);

        // Select Background colour
        if (bg == 0){
            c1->SetFillColor(1);    
            for(int i = 1; i <=30; i++){
                c1->GetPad(i)->SetFrameFillColor(1);
                c1->GetPad(i)->SetFillColor(1);
            }
            c1->SetFrameFillColor(1);
      
            c2->SetFillColor(1);
            for(int i = 1; i <=30; i++){
                c2->GetPad(i)->SetFrameFillColor(1);
                c2->GetPad(i)->SetFillColor(1);
            }
            c2->SetFrameFillColor(1);
        }
        if (bg == 1){
            c1->SetFillColor(0);    
            for(int i = 1; i <=30; i++){
                c1->GetPad(i)->SetFrameFillColor(0);
                c1->GetPad(i)->SetFillColor(0);
            }
            c1->SetFrameFillColor(0);
      
            c2->SetFillColor(0);
            for(int i = 1; i <=30; i++){
                c2->GetPad(i)->SetFrameFillColor(0);
                c2->GetPad(i)->SetFillColor(0);
            }
            c2->SetFrameFillColor(0);
        }
    }
    // Show one projection
    else{

        // Select Background colour
        if (bg == 0){
            c1->SetFillColor(1); 
            c1->SetFrameFillColor(1);
            c2->SetFrameFillColor(1);
            c2->SetFillColor(1);
        }
        if (bg == 1){
            c1->SetFillColor(0); 
            c1->SetFrameFillColor(0);
            c2->SetFrameFillColor(0);
            c2->SetFillColor(0);
        }
    }
    //-------------------------------------


    // Loop through Head0
    cout << "Plotting " << projName << "/" << type << "/Head0/proj-* on Head0" << endl; 
    cout << "Plotting " << projName << "/" << type << "/Head1/proj-* on Head1" << endl; 

    // Plot all SF
    if(proj < 0){
        for (int i = 0; i < 30; i++){
            sprintf(hname,"%s/%s/Head0/proj-%i",projName,type,i);
            gDirectory->GetObject(hname,hist);
      
            if(hist == NULL){
                cout << "ERROR: Can not get histogram - " << hname << endl;
            }
            else{
                c1->cd(i+1);
                hist->Draw("COLZ");
                hist->SetStats(0);

                // Set Background colour
                if(bg == 0){
                    hist->GetXaxis()->SetAxisColor(0);
                    hist->GetYaxis()->SetAxisColor(0);
                    hist->GetXaxis()->SetLabelColor(0);
                    hist->GetYaxis()->SetLabelColor(0);

                    gPad->Update();
                    colpal = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
                    colpal->SetLabelColor(0);
                }
                if(bg == 1){
                    hist->GetXaxis()->SetAxisColor(1);
                    hist->GetYaxis()->SetAxisColor(1);
                    hist->GetXaxis()->SetLabelColor(1);
                    hist->GetYaxis()->SetLabelColor(1);

                    gPad->Update();
                    colpal = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
                    colpal->SetLabelColor(1);
                }

                // Do we want to show the source as well ?
                if(source == 1){
                    sprintf(hname,"sim52.6MBq_v2_ALL/SourceXZ/Head0/proj-%i",i);
                    gDirectory->GetObject(hname,hist);
      
                    if(hist == NULL){
                        cout << "ERROR: Can not get histogram - " << hname << endl;
                    }
                    else{
                        cout << "Plotting Source" << endl;
                        hist->SetMarkerColor(0);
                        hist->SetTitle();
                        hist->Draw("P SAME");
                    }
                }

            }
        }
    }  
    // Just show one
    else{
        sprintf(hname,"%s/%s/Head0/proj-%i",projName,type,proj);
        gDirectory->GetObject(hname,hist);
        if(hist == NULL){
            cout << "ERROR: Can not get histogram - " << hname << endl;
        }
        else{

            c1->cd(0);
            hist->Draw("COLZ");
            hist->SetStats(0);

            // Set Background colour
            if(bg == 0){
                hist->GetXaxis()->SetAxisColor(0);
                hist->GetYaxis()->SetAxisColor(0);
                hist->GetXaxis()->SetLabelColor(0);
                hist->GetYaxis()->SetLabelColor(0);

                gPad->Update();
                colpal = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
                colpal->SetLabelColor(0);
            }
            if(bg == 1){
                hist->GetXaxis()->SetAxisColor(1);
                hist->GetYaxis()->SetAxisColor(1);
                hist->GetXaxis()->SetLabelColor(1);
                hist->GetYaxis()->SetLabelColor(1);

                gPad->Update();
                colpal = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
                colpal->SetLabelColor(1);
            }

            // Do we want to show the source as well ?
            if(source == 1){
                sprintf(hname,"sim52.6MBq_v2_ALL/SourceXZ/Head0/proj-%i",proj);
                gDirectory->GetObject(hname,hist);
      
                if(hist == NULL){
                    cout << "ERROR: Can not get histogram - " << hname << endl;
                }
                else{
                    cout << "Plotting Source" << endl;
                    hist->SetMarkerColor(0);
                    hist->SetTitle();
                    hist->Draw("P SAME");
                }
            }

        }
    }
    c1->cd(0);

    // Loop through Head1// Plot all SF
    if(proj < 0){
        for (int i = 0; i < 30; i++){
            sprintf(hname,"%s/%s/Head1/proj-%i",projName,type,i);
            gDirectory->GetObject(hname,hist);
      
            if(hist == NULL){
                cout << "ERROR: Can not get histogram - " << hname << endl;
            }
            else{
                c2->cd(i+1);
                hist->Draw("COLZ");
                hist->SetStats(0);

                // Set Background colour
                if(bg == 0){
                    hist->GetXaxis()->SetAxisColor(0);
                    hist->GetYaxis()->SetAxisColor(0);
                    hist->GetXaxis()->SetLabelColor(0);
                    hist->GetYaxis()->SetLabelColor(0);
                    hist->SetStats(0);

                    gPad->Update();
                    colpal = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
                    colpal->SetLabelColor(0);
                }
                if(bg == 1){
                    hist->GetXaxis()->SetAxisColor(1);
                    hist->GetYaxis()->SetAxisColor(1);
                    hist->GetXaxis()->SetLabelColor(1);
                    hist->GetYaxis()->SetLabelColor(1);
                    hist->SetStats(0);

                    gPad->Update();
                    colpal = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
                    colpal->SetLabelColor(1);
                }

                // Do we want to show the source as well ?
                if(source == 1){
                    sprintf(hname,"sim52.6MBq_v2_ALL/SourceXZ/Head1/proj-%i",i);
                    gDirectory->GetObject(hname,hist);
      
                    if(hist == NULL){
                        cout << "ERROR: Can not get histogram - " << hname << endl;
                    }
                    else{
                        cout << "Plotting Source" << endl;
                        hist->SetMarkerColor(0);
                        hist->SetTitle();
                        hist->Draw("P SAME");
                    }
                }

            }
        }
    
    }
    else{
        sprintf(hname,"%s/%s/Head1/proj-%i",projName,type,proj);
        gDirectory->GetObject(hname,hist);

        if(hist == NULL){
            cout << "ERROR: Can not get histogram - " << hname << endl;
        }
        else{
            c2->cd(0);
            hist->Draw("COLZ");
            hist->SetStats(0);

            // Set Background colour
            if(bg == 0){
                hist->GetXaxis()->SetAxisColor(0);
                hist->GetYaxis()->SetAxisColor(0);
                hist->GetXaxis()->SetLabelColor(0);
                hist->GetYaxis()->SetLabelColor(0);

                gPad->Update();
                colpal = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
                colpal->SetLabelColor(0);
            }
            if(bg == 1){
                hist->GetXaxis()->SetAxisColor(1);
                hist->GetYaxis()->SetAxisColor(1);
                hist->GetXaxis()->SetLabelColor(1);
                hist->GetYaxis()->SetLabelColor(1);

                gPad->Update();
                colpal = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
                colpal->SetLabelColor(1);
            }

            // Do we want to show the source as well ?
            if(source == 1){
                sprintf(hname,"sim52.6MBq_v2_ALL/SourceXZ/Head1/proj-%i",proj);
                gDirectory->GetObject(hname,hist);
      
                if(hist == NULL){
                    cout << "ERROR: Can not get histogram - " << hname << endl;
                }
                else{
                    cout << "Plotting Source" << endl;
                    hist->SetMarkerColor(0);
                    hist->SetTitle();
                    hist->Draw("P SAME");
                }
            }

        }
    }
    c2->cd(0);

}


// Show projection
void showProj(char projName[100], char type[100], int pal = 0, int proj = -99){

    char hname[300];
    TH2 *hist;

    //--- Define a user defined palette ---
    // 100 step from Black(0) to white (1)
    TColor *color;
    Int_t palette[100];
    int i,j;
    double Wcol;
  
    // color 51-299 are available ?
    for (int i=0;i<100;i++) {
        Wcol = Float_t(i)/99;
        color = new TColor(1000+i,Wcol,Wcol,Wcol,"");
        palette[i] = 1000+i;
    }
    gStyle->SetPalette(100,palette);

    // Set multi coloured palette if requested
    if (pal == 1){
        gStyle->SetPalette(1);
    }
    //-------------------------------------

    //-- Set up canvases ------------------

    Double_t w = 800;
    Double_t h = 960;
    TCanvas * c1 = new TCanvas("head0", "head0", w, h);
    c1->SetWindowSize(w + (w - c1->GetWw()), h + (h - c1->GetWh()));

    TCanvas * c2 = new TCanvas("head1", "head1", w, h);
    c2->SetWindowSize(w + (w - c2->GetWw()), h + (h - c2->GetWh()));

    // Show all 60 projections
    if (proj < 0){
    
        c1->Divide(5,6);
    
        c1->SetFillColor(1);
    
        for(int i = 1; i <=30; i++){
            c1->GetPad(i)->SetFrameFillColor(1);
            c1->GetPad(i)->SetFillColor(1);
        }
    
        c1->SetFrameFillColor(1);
    
        c2->Divide(5,6);
    
        c2->SetFillColor(1);
    
        for(int i = 1; i <=30; i++){
            c2->GetPad(i)->SetFrameFillColor(1);
            c2->GetPad(i)->SetFillColor(1);
        }
    
        c2->SetFrameFillColor(1);
    
    }
    // Show one projection
    else{
    
        c1->SetFillColor(1); 
        c1->SetFrameFillColor(1);
        c2->SetFrameFillColor(1);
        c2->SetFillColor(1);
 
    }
    //-------------------------------------


    // Loop through Head0
    cout << "Plotting " << projName << "/" << type << "/Head0/proj-* on Head0" << endl; 
    cout << "Plotting " << projName << "/" << type << "/Head1/proj-* on Head1" << endl; 

    // Plot all SF
    if(proj < 0){
        for (int i = 0; i < 30; i++){
            sprintf(hname,"%s/%s/Head0/proj-%i",projName,type,i);
            gDirectory->GetObject(hname,hist);
      
            if(hist == NULL){
                cout << "ERROR: Can not get histogram - " << hname << endl;
            }
            else{
                c1->cd(i+1);
                hist->Draw("COL");
                hist->GetXaxis()->SetAxisColor(0);
                hist->GetYaxis()->SetAxisColor(0);
                hist->GetXaxis()->SetLabelColor(0);
                hist->GetYaxis()->SetLabelColor(0);
                hist->SetStats(0);
            }
        }
    }
    else{
        sprintf(hname,"%s/%s/Head0/proj-%i",projName,type,proj);
        gDirectory->GetObject(hname,hist);

        if(hist == NULL){
            cout << "ERROR: Can not get histogram - " << hname << endl;
        }
        else{
            c1->cd(0);
            hist->Draw("COL");
            hist->GetXaxis()->SetAxisColor(0);
            hist->GetYaxis()->SetAxisColor(0);
            hist->GetXaxis()->SetLabelColor(0);
            hist->GetYaxis()->SetLabelColor(0);
            hist->SetStats(0);
        }

    }
    c1->cd(0);

    // Loop through Head0

    // Plot all SF
    if(proj < 0){
        for (int i = 0; i < 30; i++){
            sprintf(hname,"%s/%s/Head1/proj-%i",projName,type,i);
            gDirectory->GetObject(hname,hist);
      
            if(hist == NULL){
                cout << "ERROR: Can not get histogram - " << hname << endl;
            }
            else{
                c2->cd(i+1);
                hist->Draw("COL");
                hist->GetXaxis()->SetAxisColor(0);
                hist->GetYaxis()->SetAxisColor(0);
                hist->GetXaxis()->SetLabelColor(0);
                hist->GetYaxis()->SetLabelColor(0);
                hist->SetStats(0);
            }
        }
    }
    else{
        sprintf(hname,"%s/%s/Head1/proj-%i",projName,type,proj);
        gDirectory->GetObject(hname,hist);

        if(hist == NULL){
            cout << "ERROR: Can not get histogram - " << hname << endl;
        }
        else{
            c2->cd(0);
            hist->Draw("COL");
            hist->GetXaxis()->SetAxisColor(0);
            hist->GetYaxis()->SetAxisColor(0);
            hist->GetXaxis()->SetLabelColor(0);
            hist->GetYaxis()->SetLabelColor(0);
            hist->SetStats(0);
        }

    }

    c2->cd(0);

}


// Write projection to interfile
void writeInter(char *projName, char *type, int nProj, int mode = 0)
{

#define NPIXELS 128
#define NIMAGES 60

    // Pointers for current projection histograms
    TH1 *head0, *head1;
    char hname0[80], hname1[80];

    // Projection data
    short dataHead0[NPIXELS][NPIXELS];
    short dataHead1[NPIXELS][NPIXELS];
    short InterFileData[NIMAGES][NPIXELS][NPIXELS];

    // Maximum pixel count
    short maximumPixelCount = 0;

    // Name of output files
    char datafile[100];
    char headerfile[100];

    // Set output file names
    sprintf(datafile,"%s-%s.img",projName,type);
    sprintf(headerfile,"%s-%s.hdr",projName,type);

    // Loop through the projections we have specified
    for( int i = 0; i < nProj; i++){

        cout << "Proj: " << i << endl;

        // Set the current head histogram names
        sprintf(hname0,"%s/%s/Head0/proj-%i",projName,type,i);
        sprintf(hname1,"%s/%s/Head1/proj-%i",projName,type,i);

        // Get Histograms
        gDirectory->cd("/");

        gDirectory->GetObject(hname0,head0);
        gDirectory->GetObject(hname1,head1);

        // Find the maximum pixel count
        if(head0->GetMaximum() > maximumPixelCount) maximumPixelCount = head0->GetMaximum();
        if(head0->GetMaximum() > maximumPixelCount) maximumPixelCount = head0->GetMaximum();

        // Loop through head0 histogram and write data
        for(int ii = 0; ii < head0->GetXaxis()->GetNbins(); ii++){
            for(int jj = 0; jj < head0->GetYaxis()->GetNbins(); jj++){
                //cout << "head0: " <<  i << " - " << ii << ";" << jj << " " << head0->GetBinContent(ii,jj) << endl;
                dataHead0[ii][jj] = head0->GetBinContent(jj,ii); // data is flipped if ii == ii
            }
        }

        // Loop through head1 histogram and write data
        for(int ii = 0; ii < head1->GetXaxis()->GetNbins(); ii++){
            for(int jj = 0; jj < head1->GetYaxis()->GetNbins(); jj++){
                //cout << "head1: " << i << " - " << ii << ";" << jj << " " << head1->GetBinContent(ii,jj) << endl;
                dataHead1[ii][jj] = head1->GetBinContent(jj,ii);
            }
        }


        // Fill the correct position in the output array
        for(int ii = 0; ii < NPIXELS; ii++){
            for(int jj = 0; jj < NPIXELS; jj++){
                // First head
                InterFileData[i][ii][jj] = dataHead0[ii][jj];
                // Second Head
                InterFileData[i+nProj][ii][jj] = dataHead1[ii][jj];
            }
        }

    }

    // Write the interfile data

    // Open file to output final array
    FILE *output;
    if( (output = fopen(datafile, "wb")) == NULL) {
        printf("Error opening output\n");
        exit(EXIT_FAILURE);
    }
 
    // Write file to output
    size_t wr = fwrite(&InterFileData, 1, sizeof(InterFileData), output);

    cout << "---" << endl;
    cout << "Wote " << wr << " bytes to " << datafile << endl;


    // Now create the header file
    //
    ofstream hfile;
    hfile.open (headerfile);

    // Read data from tree
    char dirname[100];
    sprintf(dirname,"/%s",projName);
    gDirectory->cd(dirname);

    float energyCutLow, energyCutHigh;
    int nbins;
    float max_time;
    float time_step;
    int nprojections;

    ProjInfo->SetBranchAddress("nbins", &nbins);
    ProjInfo->SetBranchAddress("energyCutLow", &energyCutLow);
    ProjInfo->SetBranchAddress("energyCutHigh", &energyCutHigh);
    ProjInfo->SetBranchAddress("max_time", &max_time);
    ProjInfo->SetBranchAddress("time_step", &time_step);
    ProjInfo->SetBranchAddress("nprojections", &nprojections);

    ProjInfo->GetEntry(0);

    hfile << "!INTERFILE :=" << endl;
    hfile << "!imaging modality :=nucmed" << endl;
    hfile << "!originating system :=GENIE" << endl;
    hfile << "!version of keys :=3.3" << endl;
    hfile << "date of keys :=1992:01:01" << endl;
    hfile << "conversion program :=Genie" << endl;
    hfile << "program author :=Raja .P" << endl;
    hfile << "program version :=1.7" << endl;
    hfile << "program date :=1997:03:05" << endl;
    hfile << "!GENERAL DATA :=" << endl;
    hfile << "original institution :=University of Manchester" << endl;
    hfile << "contact person :=" << endl;
    hfile << "data description :=xxGENIE:TomoHwk LIVER xxGENIE:TomoHwk LIVER_EM" << endl;
    hfile << "!data offset in bytes :=0" << endl;
    hfile << "!name of data file :=" << datafile << endl;
    hfile << "patient name :=" << projName << "-" << type <<endl;
    hfile << "!patient ID :=" << projName << "-" << type << endl;
    hfile << "patient dob :=0000:00:00" << endl;
    hfile << "patient sex :=M" << endl;
    hfile << "!study ID :=TOMO" << endl;
    hfile << "exam type :=TOMO" << endl;
    hfile << "data compression :=none" << endl;
    hfile << "data encode :=none" << endl;
    hfile << "!GENERAL IMAGE DATA :=" << endl;
    hfile << "!type of data :=Tomographic" << endl;
    hfile << "!total number of images :=60" << endl;
    hfile << "study date :=2011:07:05" << endl;
    hfile << "study time :=16:54:36" << endl;
    hfile << "imagedata byte order :=LITTLEENDIAN" << endl;
    hfile << "number of energy windows :=1" << endl;
    hfile << "energy window [1] :=Tc99m" << endl;

    // These need to read from the projection
    hfile << "energy window lower level [1] :=" << energyCutLow << endl;
    hfile << "energy window upper level [1] :=" << energyCutHigh << endl;

    hfile << "flood corrected :=N" << endl;
    hfile << "decay corrected :=N" << endl;
    hfile << "!SPECT STUDY (general) :=" << endl;
    hfile << "number of detector heads :=1" << endl;
    hfile << "!number of images/energy window :=60" << endl;
    hfile << "!process status :=Acquired" << endl;

    // These need to read from the projection
    hfile << "!matrix size [1] :=" << nbins << endl;
    hfile << "!matrix size [2] :=" << nbins << endl;

    hfile << "!number format :=unsigned integer" << endl;
    hfile << "!number of bytes per pixel :=2" << endl;
    hfile << "scaling factor (mm/pixel) [1] :=4.41806" << endl;
    hfile << "scaling factor (mm/pixel) [2] :=4.41806" << endl;

    // Read from projection
    hfile << "!number of projections :=" << nprojections*2 << endl;

    hfile << "!extent of rotation :=360" << endl;

    // This needs to be read from projection
    hfile << "!time per projection (sec) :=" << time_step << endl;
    hfile << "study duration (sec) :=" << max_time << endl;

    hfile << "!maximum pixel count :=" << maximumPixelCount <<endl;
    hfile << "patient orientation :=feet_in" << endl;
    hfile << "patient rotation :=supine" << endl;


    if(mode == 0){
        hfile << "!SPECT STUDY (acquired data) :=" << endl;
        hfile << "!direction of rotation :=CW" << endl;
        hfile << "start angle :=0" << endl;
        hfile << "first projection angle in data set :=0" << endl;
        hfile << "acquisition mode :=stepped" << endl;
        hfile << "Centre_of_rotation :=Single_value" << endl;
        hfile << "!X_offset :=0" << endl;
        hfile << "Y_offset :=0" << endl;
        hfile << "Radius :=0" << endl;
        hfile << "orbit :=Circular" << endl;
        hfile << "preprocessed :=" << endl;
        hfile << "!END OF INTERFILE :=" << endl;
    }
    if(mode == 1){
        hfile << "!SPECT STUDY (reconstructed data) :==" << endl;
        hfile << "method of reconstruction := Unknown=" << endl;
        hfile << "!number of slices := 60=" << endl;
        hfile << "number of reference frame := 0=" << endl;
        hfile << "slice orientation := Transverse=" << endl;
        hfile << "slice thickness (pixels) := +1.000000e+00=" << endl;
        hfile << "centre-centre slice separation (pixels) := +1.000000e+00=" << endl;
        hfile << "filter name := Unknown=" << endl;
        hfile << "filter parameters := Cutoff=" << endl;
        hfile << "method of attenuation correction := measured=" << endl;
        hfile << "scatter corrected := N=" << endl;
        hfile << "oblique reconstruction := N=" << endl;
        hfile << "!END OF INTERFILE :=" << endl;
    }

    hfile.close();

    cout << "---" << endl;
    cout << "Wote header file to " << headerfile << endl;

    // Convert interfile to dicom  
    char command[300];
    sprintf(command,"medcon -f %s -c dicom",headerfile);
    cout << "Running: " << command << endl;
    gSystem->Exec(command);

    sprintf(command,"mv m000-%s-%s.dcm %s-%s.dcm", projName, type,  projName, type);
    cout << "Running: " << command << endl;
    gSystem->Exec(command);

}


// Write projection to interfile - FLOAT
void writeInterFloat(char *projName, char *type, int nProj, int mode = 0)
{

#define NPIXELS 128
#define NIMAGES 60

    // Pointers for current projection histograms
    TH1 *head0, *head1;
    char hname0[80], hname1[80];

    // Projection data
    float dataHead0[NPIXELS][NPIXELS];
    float dataHead1[NPIXELS][NPIXELS];
    float InterFileData[NIMAGES][NPIXELS][NPIXELS];

    // Maximum pixel count
    short maximumPixelCount = 0;

    // Name of output files
    char datafile[100];
    char headerfile[100];

    // Set output file names
    sprintf(datafile,"%s-%s.img",projName,type);
    sprintf(headerfile,"%s-%s.hdr",projName,type);

    // Loop through the projections we have specified
    for( int i = 0; i < nProj; i++){

        cout << "Proj: " << i << endl;

        // Set the current head histogram names
        sprintf(hname0,"%s/%s/Head0/proj-%i",projName,type,i);
        sprintf(hname1,"%s/%s/Head1/proj-%i",projName,type,i);

        // Get Histograms
        gDirectory->cd("/");

        gDirectory->GetObject(hname0,head0);
        gDirectory->GetObject(hname1,head1);

        // Find the maximum pixel count
        if(head0->GetMaximum() > maximumPixelCount) maximumPixelCount = head0->GetMaximum();
        if(head0->GetMaximum() > maximumPixelCount) maximumPixelCount = head0->GetMaximum();

        // Loop through head0 histogram and write data
        for(int ii = 0; ii < head0->GetXaxis()->GetNbins(); ii++){
            for(int jj = 0; jj < head0->GetYaxis()->GetNbins(); jj++){
                //cout << "head0: " <<  i << " - " << ii << ";" << jj << " " << head0->GetBinContent(ii,jj) << endl;
                dataHead0[ii][jj] = head0->GetBinContent(jj,ii); // data is flipped if ii == ii
            }
        }

        // Loop through head1 histogram and write data
        for(int ii = 0; ii < head1->GetXaxis()->GetNbins(); ii++){
            for(int jj = 0; jj < head1->GetYaxis()->GetNbins(); jj++){
                //cout << "head1: " << i << " - " << ii << ";" << jj << " " << head1->GetBinContent(ii,jj) << endl;
                dataHead1[ii][jj] = head1->GetBinContent(jj,ii);
            }
        }


        // Fill the correct position in the output array
        for(int ii = 0; ii < NPIXELS; ii++){
            for(int jj = 0; jj < NPIXELS; jj++){
                // First head
                InterFileData[i][ii][jj] = dataHead0[ii][jj];
                // Second Head
                InterFileData[i+nProj][ii][jj] = dataHead1[ii][jj];
            }
        }

    }

    // Write the interfile data

    // Open file to output final array
    FILE *output;
    if( (output = fopen(datafile, "wb")) == NULL) {
        printf("Error opening output\n");
        exit(EXIT_FAILURE);
    }
 
    // Write file to output
    size_t wr = fwrite(&InterFileData, 1, sizeof(InterFileData), output);

    cout << "---" << endl;
    cout << "Wote " << wr << " bytes to " << datafile << endl;


    // Now create the header file
    //
    ofstream hfile;
    hfile.open (headerfile);

    // Read data from tree
    char dirname[100];
    sprintf(dirname,"/%s",projName);
    gDirectory->cd(dirname);

    float energyCutLow, energyCutHigh;
    int nbins;
    float max_time;
    float time_step;
    int nprojections;

    ProjInfo->SetBranchAddress("nbins", &nbins);
    ProjInfo->SetBranchAddress("energyCutLow", &energyCutLow);
    ProjInfo->SetBranchAddress("energyCutHigh", &energyCutHigh);
    ProjInfo->SetBranchAddress("max_time", &max_time);
    ProjInfo->SetBranchAddress("time_step", &time_step);
    ProjInfo->SetBranchAddress("nprojections", &nprojections);

    ProjInfo->GetEntry(0);

    hfile << "!INTERFILE :=" << endl;
    hfile << "!imaging modality :=nucmed" << endl;
    hfile << "!originating system :=GENIE" << endl;
    hfile << "!version of keys :=3.3" << endl;
    hfile << "date of keys :=1992:01:01" << endl;
    hfile << "conversion program :=Genie" << endl;
    hfile << "program author :=Raja .P" << endl;
    hfile << "program version :=1.7" << endl;
    hfile << "program date :=1997:03:05" << endl;
    hfile << "!GENERAL DATA :=" << endl;
    hfile << "original institution :=University of Manchester" << endl;
    hfile << "contact person :=" << endl;
    hfile << "data description :=xxGENIE:TomoHwk LIVER xxGENIE:TomoHwk LIVER_EM" << endl;
    hfile << "!data offset in bytes :=0" << endl;
    hfile << "!name of data file :=" << datafile << endl;
    hfile << "patient name :=" << projName << "-" << type <<endl;
    hfile << "!patient ID :=" << projName << "-" << type << endl;
    hfile << "patient dob :=0000:00:00" << endl;
    hfile << "patient sex :=M" << endl;
    hfile << "!study ID :=TOMO" << endl;
    hfile << "exam type :=TOMO" << endl;
    hfile << "data compression :=none" << endl;
    hfile << "data encode :=none" << endl;
    hfile << "!GENERAL IMAGE DATA :=" << endl;
    hfile << "!type of data :=Tomographic" << endl;
    hfile << "!total number of images :=60" << endl;
    hfile << "study date :=2011:07:05" << endl;
    hfile << "study time :=16:54:36" << endl;
    hfile << "imagedata byte order :=LITTLEENDIAN" << endl;
    hfile << "number of energy windows :=1" << endl;
    hfile << "energy window [1] :=Tc99m" << endl;

    // These need to read from the projection
    hfile << "energy window lower level [1] :=" << energyCutLow << endl;
    hfile << "energy window upper level [1] :=" << energyCutHigh << endl;

    hfile << "flood corrected :=N" << endl;
    hfile << "decay corrected :=N" << endl;
    hfile << "!SPECT STUDY (general) :=" << endl;
    hfile << "number of detector heads :=1" << endl;
    hfile << "!number of images/energy window :=60" << endl;
    hfile << "!process status :=Acquired" << endl;

    // These need to read from the projection
    hfile << "!matrix size [1] :=" << nbins << endl;
    hfile << "!matrix size [2] :=" << nbins << endl;

    hfile << "!number format :=float" << endl;
    hfile << "!number of bytes per pixel :=4" << endl;
    hfile << "scaling factor (mm/pixel) [1] :=4.41806" << endl;
    hfile << "scaling factor (mm/pixel) [2] :=4.41806" << endl;

    // Read from projection
    hfile << "!number of projections :=" << nprojections*2 << endl;

    hfile << "!extent of rotation :=360" << endl;

    // This needs to be read from projection
    hfile << "!time per projection (sec) :=" << time_step << endl;
    hfile << "study duration (sec) :=" << max_time << endl;

    hfile << "!maximum pixel count :=" << maximumPixelCount <<endl;
    hfile << "patient orientation :=feet_in" << endl;
    hfile << "patient rotation :=supine" << endl;


    if(mode == 0){
        hfile << "!SPECT STUDY (acquired data) :=" << endl;
        hfile << "!direction of rotation :=CW" << endl;
        hfile << "start angle :=0" << endl;
        hfile << "first projection angle in data set :=0" << endl;
        hfile << "acquisition mode :=stepped" << endl;
        hfile << "Centre_of_rotation :=Single_value" << endl;
        hfile << "!X_offset :=0" << endl;
        hfile << "Y_offset :=0" << endl;
        hfile << "Radius :=0" << endl;
        hfile << "orbit :=Circular" << endl;
        hfile << "preprocessed :=" << endl;
        hfile << "!END OF INTERFILE :=" << endl;
    }
    if(mode == 1){
        hfile << "!SPECT STUDY (reconstructed data) :==" << endl;
        hfile << "method of reconstruction := Unknown=" << endl;
        hfile << "!number of slices := 60=" << endl;
        hfile << "number of reference frame := 0=" << endl;
        hfile << "slice orientation := Transverse=" << endl;
        hfile << "slice thickness (pixels) := +1.000000e+00=" << endl;
        hfile << "centre-centre slice separation (pixels) := +1.000000e+00=" << endl;
        hfile << "filter name := Unknown=" << endl;
        hfile << "filter parameters := Cutoff=" << endl;
        hfile << "method of attenuation correction := measured=" << endl;
        hfile << "scatter corrected := N=" << endl;
        hfile << "oblique reconstruction := N=" << endl;
        hfile << "!END OF INTERFILE :=" << endl;
    }

    hfile.close();

    cout << "---" << endl;
    cout << "Wote header file to " << headerfile << endl;

    // Convert interfile to dicom  
    char command[300];
    sprintf(command,"medcon -f %s -c dicom",headerfile);
    cout << "Running: " << command << endl;
    gSystem->Exec(command);

    sprintf(command,"mv m000-%s-%s.dcm %s-%s.dcm", projName, type,  projName, type);
    cout << "Running: " << command << endl;
    gSystem->Exec(command);

}


// Write projection to interfile using the supplied headerfile
// This lets us make projections which are fully compatibale with Xeleris !
void writeInter2(char *projName, char *type, char *header_file, char *id, int nProj)
{

#define NPIXELS 128
#define NIMAGES 60

    // Pointers for current projection histograms
    TH1 *head0, *head1;
    char hname0[80], hname1[80];

    // Projection data
    short dataHead0[NPIXELS][NPIXELS];
    short dataHead1[NPIXELS][NPIXELS];
    short InterFileData[NIMAGES][NPIXELS][NPIXELS];

    // Maximum pixel count
    short maximumPixelCount = 0;

    // Name of output files
    char datafile[100];
    char headerfile[100];

    // Set output file names
    sprintf(datafile,"%s-%s.img",projName,type);
    sprintf(headerfile,"%s-%s.hdr",projName,type);

    // Loop through the projections we have specified
    for( int i = 0; i < nProj; i++){

        cout << "Proj: " << i << endl;

        // Set the current head histogram names
        sprintf(hname0,"%s/%s/Head0/proj-%i",projName,type,i);
        sprintf(hname1,"%s/%s/Head1/proj-%i",projName,type,i);

        // Get Histograms
        gDirectory->cd("/");

        gDirectory->GetObject(hname0,head0);
        gDirectory->GetObject(hname1,head1);

        // Find the maximum pixel count
        if(head0->GetMaximum() > maximumPixelCount) maximumPixelCount = head0->GetMaximum();
        if(head0->GetMaximum() > maximumPixelCount) maximumPixelCount = head0->GetMaximum();

        // Loop through head0 histogram and write data
        for(int ii = 0; ii < head0->GetXaxis()->GetNbins(); ii++){
            for(int jj = 0; jj < head0->GetYaxis()->GetNbins(); jj++){
                //cout << "head0: " <<  i << " - " << ii << ";" << jj << " " << head0->GetBinContent(ii,jj) << endl;
                dataHead0[ii][jj] = head0->GetBinContent(jj,ii); // data is flipped if ii == ii
            }
        }

        // Loop through head1 histogram and write data
        for(int ii = 0; ii < head1->GetXaxis()->GetNbins(); ii++){
            for(int jj = 0; jj < head1->GetYaxis()->GetNbins(); jj++){
                //cout << "head1: " << i << " - " << ii << ";" << jj << " " << head1->GetBinContent(ii,jj) << endl;
                dataHead1[ii][jj] = head1->GetBinContent(jj,ii);
            }
        }


        // Fill the correct position in the output array
        for(int ii = 0; ii < NPIXELS; ii++){
            for(int jj = 0; jj < NPIXELS; jj++){
                // First head
                InterFileData[i][ii][jj] = dataHead0[ii][jj];
                // Second Head
                InterFileData[i+nProj][ii][jj] = dataHead1[ii][jj];
            }
        }

    }

    // Write the interfile data

    // Open file to output final array
    FILE *output;
    if( (output = fopen(datafile, "wb")) == NULL) {
        printf("Error opening output\n");
        exit(EXIT_FAILURE);
    }
 
    // Write file to output
    size_t wr = fwrite(&InterFileData, 1, sizeof(InterFileData), output);

    cout << "---" << endl;
    cout << "Wote " << wr << " bytes to " << datafile << endl;

    //
    // Now create the header file
    //
    ofstream hfile;
    hfile.open (headerfile);

    // Read data from tree
    char dirname[100];
    sprintf(dirname,"/%s",projName);
    gDirectory->cd(dirname);

    float energyCutLow, energyCutHigh;
    int nbins;
    float max_time;
    float time_step;
    int nprojections;

    ProjInfo->SetBranchAddress("nbins", &nbins);
    ProjInfo->SetBranchAddress("energyCutLow", &energyCutLow);
    ProjInfo->SetBranchAddress("energyCutHigh", &energyCutHigh);
    ProjInfo->SetBranchAddress("max_time", &max_time);
    ProjInfo->SetBranchAddress("time_step", &time_step);
    ProjInfo->SetBranchAddress("nprojections", &nprojections);

    ProjInfo->GetEntry(0);

    // Read the header file we wish to use
    ifstream headerFile;
    string line;
    headerFile.open(header_file);
    if(!headerFile){
        cout << "Error opening header file " << header_file << endl;
        exit(1);
    }

    // Loop through header file
    while(!headerFile.eof()){

        // Get the next line
        getline(headerFile,line);
    
        // Check if it is a line we need to change...
        size_t found;

        int matched = 0;

        found=line.find("name of data file");
        if (found!=string::npos){
            hfile << "!name of data file :=" << datafile << "\r\n";
            matched++;
        }
        found=line.find("data description");
        if (found!=string::npos){

            // Get the original description
            string one = line.substr(19,line.length()-20);
            hfile << "data description :=" << one << id << "\r\n";
            //      hfile << "***********" << one << endl;
            matched++;
        }
        found=line.find("energy window lower level"); 
        if (found!=string::npos){
            hfile << "energy window lower level [1] :=" << energyCutLow*1000.0 << "\r\n";
            matched++;
        }
        found=line.find("energy window upper level"); 
        if (found!=string::npos){
            hfile << "energy window upper level [1] :=" << energyCutHigh*1000.0 << "\r\n";
            matched++;
        }
//     found=line.find("matrix size [1]");
//     if (found!=string::npos){
//       hfile << "!matrix size [1] :=" << nbins << "\r\n";
//       matched++;
//     }
//     found=line.find("matrix size [2]");
//     if (found!=string::npos){
//       hfile << "!matrix size [2] :=" << nbins << "\r\n";
//       matched++;
//     }
//     found=line.find("number of projections");
//     if (found!=string::npos){
//       hfile << "!number of projections :=" << nprojections*2 << "\r\n";   
//       matched++;
//     }
//     found=line.find("time per projection");
//     if (found!=string::npos){
//       hfile << "!time per projection (sec) :=" << time_step << "\r\n";
//       matched++;
//     }
//     found=line.find("study duration (sec)");
//     if (found!=string::npos){
//       hfile << "study duration (sec) :=" << max_time << "\r\n";
//       matched++;
//     }
        found=line.find("maximum pixel count");
        if (found!=string::npos){
            hfile << "!maximum pixel count :=" << maximumPixelCount << "\r\n";
            matched++;
        }
        // else copy the line directly...
        if(matched == 0){
            hfile << line << endl;
        }
    }
    //  hfile << "!INTERFILE :=" << endl;
    //  hfile << "!imaging modality :=nucmed" << endl;
    //  hfile << "!originating system :=GENIE" << endl;
    //  hfile << "!version of keys :=3.3" << endl;
    //  hfile << "date of keys :=1992:01:01" << endl;
    //  hfile << "conversion program :=Genie" << endl;
    //  hfile << "program author :=Raja .P" << endl;
    //  hfile << "program version :=1.7" << endl;
    //  hfile << "program date :=1997:03:05" << endl;
    //  hfile << "!GENERAL DATA :=" << endl;
    //  hfile << "original institution :=University of Manchester" << endl;
    //  hfile << "contact person :=" << endl;
    //  hfile << "data description :=xxGENIE:TomoHwk LIVER xxGENIE:TomoHwk LIVER_EM" << endl;
    //  hfile << "!data offset in bytes :=0" << endl;
    //  hfile << "!name of data file :=" << datafile << endl;
    //  hfile << "patient name :=" << projName << "-" << type <<endl;
    //  hfile << "!patient ID :=" << projName << "-" << type << endl;
    //  hfile << "patient dob :=0000:00:00" << endl;
    //  hfile << "patient sex :=M" << endl;
    //  hfile << "!study ID :=TOMO" << endl;
    //  hfile << "exam type :=TOMO" << endl;
    //  hfile << "data compression :=none" << endl;
    //  hfile << "data encode :=none" << endl;
    //  hfile << "!GENERAL IMAGE DATA :=" << endl;
    //  hfile << "!type of data :=Tomographic" << endl;
    //  hfile << "!total number of images :=60" << endl;
    //  hfile << "study date :=2011:07:05" << endl;
    //  hfile << "study time :=16:54:36" << endl;
    //  hfile << "imagedata byte order :=LITTLEENDIAN" << endl;
    //  hfile << "number of energy windows :=1" << endl;
    //  hfile << "energy window [1] :=Tc99m" << endl;

    //  // These need to read from the projection
    //  hfile << "energy window lower level [1] :=" << energyCutLow << endl;
    //  hfile << "energy window upper level [1] :=" << energyCutHigh << endl;

    //  hfile << "flood corrected :=N" << endl;
    //  hfile << "decay corrected :=N" << endl;
    //  hfile << "!SPECT STUDY (general) :=" << endl;
    //  hfile << "number of detector heads :=1" << endl;
    //  hfile << "!number of images/energy window :=60" << endl;
    //  hfile << "!process status :=Acquired" << endl;

    //  // These need to read from the projection
    //  hfile << "!matrix size [1] :=" << nbins << endl;
    //  hfile << "!matrix size [2] :=" << nbins << endl;

    //  hfile << "!number format :=unsigned integer" << endl;
    //  hfile << "!number of bytes per pixel :=2" << endl;
    //  hfile << "scaling factor (mm/pixel) [1] :=4.41806" << endl;
    //  hfile << "scaling factor (mm/pixel) [2] :=4.41806" << endl;

    //  // Read from projection
    //  hfile << "!number of projections :=" << nprojections*2 << endl;

    //  hfile << "!extent of rotation :=360" << endl;

    // // This needs to be read from projection
    //  hfile << "!time per projection (sec) :=" << time_step << endl;
    //  hfile << "study duration (sec) :=" << max_time << endl;

    //  hfile << "!maximum pixel count :=" << maximumPixelCount <<endl;
    //  hfile << "patient orientation :=feet_in" << endl;
    //  hfile << "patient rotation :=supine" << endl;


    //  if(mode == 0){
    //    hfile << "!SPECT STUDY (acquired data) :=" << endl;
    //    hfile << "!direction of rotation :=CW" << endl;
    //    hfile << "start angle :=0" << endl;
    //    hfile << "first projection angle in data set :=0" << endl;
    //    hfile << "acquisition mode :=stepped" << endl;
    //    hfile << "Centre_of_rotation :=Single_value" << endl;
    //    hfile << "!X_offset :=0" << endl;
    //    hfile << "Y_offset :=0" << endl;
    //    hfile << "Radius :=0" << endl;
    //    hfile << "orbit :=Circular" << endl;
    //    hfile << "preprocessed :=" << endl;
    //    hfile << "!END OF INTERFILE :=" << endl;
    //  }
    //  if(mode == 1){
    //    hfile << "!SPECT STUDY (reconstructed data) :==" << endl;
    //    hfile << "method of reconstruction := Unknown=" << endl;
    //    hfile << "!number of slices := 60=" << endl;
    //    hfile << "number of reference frame := 0=" << endl;
    //    hfile << "slice orientation := Transverse=" << endl;
    //    hfile << "slice thickness (pixels) := +1.000000e+00=" << endl;
    //    hfile << "centre-centre slice separation (pixels) := +1.000000e+00=" << endl;
    //    hfile << "filter name := Unknown=" << endl;
    //    hfile << "filter parameters := Cutoff=" << endl;
    //    hfile << "method of attenuation correction := measured=" << endl;
    //    hfile << "scatter corrected := N=" << endl;
    //    hfile << "oblique reconstruction := N=" << endl;
    //    hfile << "!END OF INTERFILE :=" << endl;
    //  }

    hfile.close();

    cout << "---" << endl;
    cout << "Wote header file to " << headerfile << endl;

    // Convert interfile to dicom  
    char command[300];
    sprintf(command,"medcon -f %s -c dicom",headerfile);
    cout << "Running: " << command << endl;
    gSystem->Exec(command);

    sprintf(command,"mv m000-%s-%s.dcm %s-%s.dcm", projName, type,  projName, type);
    cout << "Running: " << command << endl;
    gSystem->Exec(command);

}
